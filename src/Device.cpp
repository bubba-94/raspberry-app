#include "Device.hpp"

Device::Device() : state{true} {}
Device::~Device() {
  state = false;
  if (worker.joinable()) {
    std::cout << "Joined\n";
    worker.join();
  }
  close(port);
}

void Device::init() {
  if (!connectToPort()) {
    std::cout << "Port connection failed\n";
  }

  worker = std::thread(&Device::poll, this);
}
void Device::setup() {}

void Device::poll() { readFromSerial(); }

void Device::readFromSerial() {

  char c;
  while (state) {
    int bytes = read(port, &c, 1);

    if (bytes <= 0)
      continue;

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

bool Device::connectToPort() {

  // Open port before configuration
  port = open(PORT_A, O_RDWR | O_NOCTTY);
  if (port < 0) {
    std::cout << "Port opening failed\n";
    return -1;
  }
  struct termios pts;

  if (tcgetattr(port, &pts) != 0) {
    std::cout << "Existing settings not read\n";
    return -1;
  }
  // Terminal confiuration instance
  configureSerial(pts, 9600);

  if (tcsetattr(port, TCSANOW, &pts) != 0) {
    std::cout << "Saving new setting not successful";
    return -1;
  }

  std::cout << PORT_A << "is open." << "\n";

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

void Device::sleepFor(uint8_t delay) {
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

uint16_t Device::getWeight() { return weight; }

uint16_t Device::convertWeight() {
  weight = std::stoi(incomingWeight);
  return weight;
}