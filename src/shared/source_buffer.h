#ifndef VELA_SOURCE_BUFFER_H
#define VELA_SOURCE_BUFFER_H

#include <concepts>
#include <cstring>
#include <fstream>
#include <memory>
#include <optional>
#include <type_traits>

#include "factories.h"

namespace vela::shared {
    template <typename TAlign> requires requires { { alignof(TAlign) } -> std::same_as<std::size_t>; }
    class PaddedSourceBufferView {
    public:
        static std::optional<PaddedSourceBufferView> load(const char *path, const long long maxFileSize) {
            std::ifstream file(path, std::ios::in | std::ios::binary);
            if (!file) {
                throw std::runtime_error("failed to open file: " + std::string(path));
            }

            constexpr size_t alignment = alignof(TAlign);
            const std::streamoff fileSize = file.tellg();
            const size_t paddedSize = (fileSize + alignment - 1) & ~(alignment - 1);
            if (paddedSize > maxFileSize) {
                throw std::runtime_error("file padded to next boundary is too big: " + std::string(path));
            }

            auto data = adopt_unique_with_deleter<uint8_t[], Free<TAlign>>(
                static_cast<uint8_t *>(operator new(paddedSize, static_cast<std::align_val_t>(alignment))),
                Free<TAlign>{}
            );

            file.read(reinterpret_cast<char *>(data.get()), fileSize);
            if (file.bad()) {
                throw std::runtime_error("failed to read file: " + std::string(path));
            }

            const auto bytesRead = static_cast<size_t>(file.gcount());
            std::memset(data.get() + bytesRead, 0, paddedSize - bytesRead);

            return PaddedSourceBufferView(std::move(data), paddedSize);
        }

        // clang-format off
        template<typename T>
            requires std::is_pointer_v<T> && std::is_const_v<std::remove_pointer_t<T>>
        T data() const { return m_data.get(); }

        [[nodiscard]] long long size() const { return m_paddedSize; }
        // cland-format on

    private:
        template <typename T>
        struct Free {
            void operator()(uint8_t *data) const noexcept {
                operator delete[](data, static_cast<std::align_val_t>(alignof(T)));
            }
        };

        PaddedSourceBufferView(std::unique_ptr<uint8_t[], Free<TAlign>> &&data, const long long size)
            : m_data(std::move(data)), m_paddedSize(size) { }

        std::unique_ptr<uint8_t[], Free<TAlign>> m_data;
        long long m_paddedSize = 0;
    };
} // namespace vela::shared

#endif // VELA_SOURCE_BUFFER_H
