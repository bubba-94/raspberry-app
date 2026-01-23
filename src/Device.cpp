#include "Device.hpp"

Device::Device() : state{true} {
  if (!connectToPort()) {
    std::cout << "[Device] Port connection failed\n";
  }

  worker = std::thread(&Device::poll, this);
}
Device::~Device() {
  state = false;
  if (worker.joinable()) {
    std::cout << "[Device] Thread joined..\n";
    worker.join();
  }
  close(fd);
}

void Device::poll() { readFromSerial(); }
uint16_t Device::getWeight() { return weight; }

uint16_t Device::convertWeight() {
  weight = std::stoi(incomingWeight);
  return weight;
}

void Device::readFromSerial() {

  char c;
  while (state) {
    int bytes = read(fd, &c, 1);

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