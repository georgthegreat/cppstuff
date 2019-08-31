#include <filesystem>
#include <string_view>

template<typename... Parts>
std::filesystem::path join_path(std::filesystem::path origin, std::string_view part, const Parts&... parts) {
    origin.append(part);
    (origin.append(parts), ...);
    return origin;
}
