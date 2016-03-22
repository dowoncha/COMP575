#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  std::ofstream ofs("test.ppm", std::ios::out | std::ios::binary);

  // write headers
  ofs << "P6"			  << '\n'
      << 512    << ' '
      << 512   << '\n'
      << 255			  << '\n';

      for (int i = 0; i < 512 * 512; ++i)
      {
        ofs << (uint8_t)255 << (uint8_t)255 << (uint8_t)0;
      }

      ofs.close();

      return 0;
}
