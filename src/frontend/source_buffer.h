// ReSharper disable CppPassValueParameterByConstReference
#ifndef VELA_SOURCE_BUFFER_H
#define VELA_SOURCE_BUFFER_H

#include <concepts>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>

#include "shared/helpers.h"

namespace vela::shared {
    struct SourceBufferCreationConfig {
        long long paddingBeyondBoundary;
        uint8_t additionalPaddingValue;
        long long maxFileSize;
    };

    template<typename TAlign>
        requires requires {
            { alignof(TAlign) } -> std::same_as<size_t>;
        }
    class FileSourceBuffer {
    private:
        template<typename T>
        struct Free {
            void operator()(uint8_t *data) const noexcept {
                operator delete(data, std::align_val_t{alignof(T)});
            }
        };

    public:
        static FileSourceBuffer load(const char *path, SourceBufferCreationConfig config) {
            std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
            if (!file) {
                throw std::runtime_error("failed to open file: " + std::string(path));
            }

            const std::streamoff pos = file.tellg();
            if (pos < 0) {
                throw std::runtime_error("failed to get file size: " + std::string(path));
            }
            file.seekg(0, std::ios::beg);

            constexpr std::size_t alignment = alignof(TAlign);
            const auto rawSize = static_cast<std::size_t>(pos);
            const std::size_t alignedSize = (rawSize + alignment - 1) & ~(alignment - 1);
            const std::size_t totalSize = alignedSize + static_cast<std::size_t>(config.paddingBeyondBoundary);

            if (totalSize > static_cast<std::size_t>(config.maxFileSize)) {
                throw std::runtime_error("file with padding is too big: " + std::string(path));
            }

            auto data = adopt_unique_with_deleter<uint8_t[], Free<TAlign>>(
                static_cast<uint8_t *>(operator new(totalSize, std::align_val_t{alignment})),
                Free<TAlign>{}
            );

            file.read(reinterpret_cast<char *>(data.get()), static_cast<std::streamsize>(rawSize));
            if (file.bad()) {
                throw std::runtime_error("failed to read file: " + std::string(path));
            }

            const auto bytesRead = static_cast<std::size_t>(file.gcount());
            std::memset(data.get() + bytesRead, config.additionalPaddingValue, totalSize - bytesRead);

            return FileSourceBuffer(std::move(data), config, bytesRead, alignedSize, totalSize);
        }

        static FileSourceBuffer from_string(const std::string_view src, SourceBufferCreationConfig config) {
            constexpr std::size_t alignment = alignof(TAlign);
            const auto rawSize = src.size();
            const std::size_t alignedSize = (rawSize + alignment - 1) & ~(alignment - 1);
            const std::size_t totalSize = alignedSize + static_cast<std::size_t>(config.paddingBeyondBoundary);

            if (totalSize > static_cast<std::size_t>(config.maxFileSize)) {
                throw std::runtime_error("string source with padding is too big");
            }

            auto data = adopt_unique_with_deleter<uint8_t[], Free<TAlign>>(
                static_cast<uint8_t *>(operator new(totalSize, std::align_val_t{alignment})),
                Free<TAlign>{}
            );

            std::memcpy(data.get(), src.data(), rawSize);
            std::memset(data.get() + rawSize, config.additionalPaddingValue, totalSize - rawSize);

            return FileSourceBuffer(std::move(data), config, rawSize, alignedSize, totalSize);
        }

        static FileSourceBuffer from_subset(
            const FileSourceBuffer &src,
            std::size_t start,
            std::size_t end)
        {
            if (start > end || end > src.rawSize()) {
                throw std::out_of_range("subset is out of range");
            }

            constexpr std::size_t alignment = alignof(TAlign);
            const auto rawSize = end - start;
            const std::size_t alignedSize = (rawSize + alignment - 1) & ~(alignment - 1);
            const std::size_t totalSize = alignedSize + static_cast<std::size_t>(src.m_config.paddingBeyondBoundary);

            if (totalSize > static_cast<std::size_t>(src.m_config.maxFileSize)) {
                throw std::runtime_error("source buffer subset with padding is too big");
            }

            auto data = adopt_unique_with_deleter<uint8_t[], Free<TAlign>>(
                static_cast<uint8_t *>(operator new(totalSize, std::align_val_t{alignment})),
                Free<TAlign>{}
            );

            std::memcpy(data.get(), src.data() + start, rawSize);
            std::memset(data.get() + rawSize, src.m_config.additionalPaddingValue, totalSize - rawSize);

            return FileSourceBuffer(std::move(data), src.m_config, rawSize, alignedSize, totalSize);
        }

        // clang-format off
        [[nodiscard]] const uint8_t *data() const { return m_data.get(); }

        [[nodiscard]] std::size_t rawSize() const { return m_rawSize; }
        [[nodiscard]] std::size_t alignedSize() const { return m_alignedSize; }
        [[nodiscard]] std::size_t totalSize() const { return m_totalSize; }
        // clang-format on

    private:
        // clang-format off
        FileSourceBuffer(std::unique_ptr<uint8_t[], Free<TAlign>> &&data, SourceBufferCreationConfig config,
                         const std::size_t rawSize, const std::size_t alignedSize, const std::size_t totalSize)
            : m_data(std::move(data)),
              m_config(config),
              m_rawSize(rawSize),
              m_alignedSize(alignedSize),
              m_totalSize(totalSize) { }
        // clang-format on

        std::unique_ptr<uint8_t[], Free<TAlign>> m_data;
        SourceBufferCreationConfig m_config;
        std::size_t m_rawSize = 0;
        std::size_t m_alignedSize = 0;
        std::size_t m_totalSize = 0;
    };
} // namespace vela::shared

#endif // VELA_SOURCE_BUFFER_H
