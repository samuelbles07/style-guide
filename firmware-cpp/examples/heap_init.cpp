#include <new>

#include <memory>
#include <vector>

class Packet {
public:
  explicit Packet(size_t size_bytes) : _data(size_bytes, 0u) {}

private:
  std::vector<uint8_t> _data;
};

// Heap is allowed when intentional.
// Prefer allocating during init and reusing objects in steady-state.
std::unique_ptr<Packet> make_packet_for_init(size_t size_bytes) {
  Packet *raw = new (std::nothrow) Packet(size_bytes);
  return std::unique_ptr<Packet>(raw);
}
