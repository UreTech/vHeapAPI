#define PACKET_SIZE_16B  16
#define PACKET_SIZE_32B  32
#define PACKET_SIZE_64B  64
#define PACKET_SIZE_128B 128
#define PACKET_SIZE_256B 256
#define PACKET_SIZE_512B 512
#define PACKET_SIZE_1K   1024
//not stable
#define PACKET_SIZE_2K   1024*2
#define PACKET_SIZE_4K   1024*4
#define PACKET_SIZE_8K   1024*8
#define PACKET_SIZE_16K  1024*16
#define PACKET_SIZE_32K  1024*32
#define PACKET_SIZE_64K  1024*64
#define PACKET_SIZE_128K 1024*128

//configs
#define MAX_PACKET_SIZE PACKET_SIZE_32B //smaller is more stable
#define TIMEOUT_VALUE_MS 15000 // 15s

//vhp
#define vhp_version_str  "v.1.2.0"
#define vhp_version_desc "vSugar"

#define vhp_unknown_data_size -1
#define vhp_error_data_size -2

//nhp
#define nhp_version_str  "n.1.0.0"
#define nhp_version_desc "nSugar"

//util
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#define FILE_NAME_ONLY (std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/\\") + 1))

#define LOG_INFO(msg) \
    std::cout << "[" << GREEN << "INFO" << RESET << "][" << BLUE << FILE_NAME_ONLY << RESET << "](" << BLUE << "line:" \
              << __LINE__ << RESET << "): " << msg << std::endl;

#define LOG_WARN(msg) \
    std::cout << "[" << YELLOW << "WARN" << RESET << "][" << BLUE << FILE_NAME_ONLY << RESET << "](" << BLUE << "line:" \
              << __LINE__ << RESET << "): " << msg << std::endl;

#define LOG_ERROR(msg) \
    std::cout << "[" << RED << "ERROR" << RESET << "][" << BLUE << FILE_NAME_ONLY << RESET << "](" << BLUE << "line:" \
              << __LINE__ << RESET << "): " << msg << std::endl;