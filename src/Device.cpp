#include "Device.hpp"

Device::Device() : state{true} {
  if (!connectToPort()) {
    std::cout << "[Device] Port connection failed\n";
  }

  // Start working threads
  workers.emplace_back(&Device::pollWeight, this);
  workers.emplace_back(&Device::pollTime, this);
}
Device::~Device() {

  // End sessions
  state = false;

  close(fd);

  // Join all threads
  for (auto &thread : workers) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void Device::pollWeight() { readFromSerial(); }
void Device::pollTime() { setTime(); }

uint16_t Device::getWeight() { return weight; }
std::string_view Device::getTimepoint() const { return timepoint; }

uint16_t Device::convertWeight() {
  weight = std::stoi(incomingWeight);
  return weight;
}

void Device::readFromSerial() {

  char c;
  while (state.load()) {
    int bytes = read(fd, &c, 1);

    if (bytes <= 0)
      break;

    std::lock_guard<std::mutex> lock(mutex);

    // Clear on end of line
    if (c == '\n' || c == '\r') {
      // Convert to int
      if (!incomingWeight.empty()) {
        convertWeight();
        incomingWeight.clear();
      }
    } else {
      incomingWeight += c;
    }
  }
}

void Device::setTime() {
  while (state.load()) {

    // Used for measuring clock updates
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    {
      // Aquire lock for updates.
      std::tm tm{};
      localtime_r(&t, &tm);
      std::lock_guard<std::mutex> lock(mutex);
      std::strftime(std::data(timeString), std::size(timeString),
                    "%d/%m-%y %H:%M", &tm);

      timepoint = timeString;
    }

    // Align to next real-world minute
    auto nextMinute = std::chrono::time_point_cast<std::chrono::minutes>(now) +
                      std::chrono::minutes(1);

    std::cout << "[Device] " << timeString << "\n";

    // Shorten the sleep if needed. (shutdown)
    while (state.load() && std::chrono::system_clock::now() < nextMinute) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }
}

bool Device::connectToPort() {

  // Open port before configuration
  fd = open(PORT_A, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    std::cout << "[Device] Port opening failed\n";
    return false;
  }
  struct termios pts;

  if (tcgetattr(fd, &pts) != 0) {
    std::cout << "[Device] Existing settings not read\n";
    return false;
  }
  // Terminal confiuration instance
  configureSerial(pts, 9600);

  if (tcsetattr(fd, TCSANOW, &pts) != 0) {
    std::cout << "[Device] Saving new setting not successful";
    return false;
  }

  std::cout << "[Device] " << PORT_A << " is open." << "\n";

  return true;
}

void Device::configureSerial(termios &settings, int baud) {
  // Control modes (how the data is packed)
  // Bit clearing (off)
  settings.c_cflag &= ~(PARENB | CSIZE | CRTSCTS);

  // Bit setting (on)
  settings.c_cflag |= (CS8 | CREAD | CLOCAL);

  // Local modes (how data is interpreted locally)
  // Clear local modes
  settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);

  // Input modes
  // Clear special handling of bytes
  settings.c_iflag &= ~(IXON | IXOFF | IXANY);
  settings.c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

  // Output modes
  // Clear special handling of bytes
  settings.c_oflag &= ~(OPOST | ONLCR);

  settings.c_cc[VTIME] = 10;
  settings.c_cc[VMIN] = 0;

  cfsetispeed(&settings, baud);
  cfsetospeed(&settings, baud);
}