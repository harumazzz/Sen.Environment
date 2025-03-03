#pragma once

#include "mapped_stream.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel {
    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint8MemoryStream = MappedStream<mode, u8, MappedMemory<u8> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint16MemoryStream = MappedStream<mode, u16, MappedMemory<u16> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint32MemoryStream = MappedStream<mode, u32, MappedMemory<u32> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint64MemoryStream = MappedStream<mode, u64, MappedMemory<u64> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int8MemoryStream = MappedStream<mode, i8, MappedMemory<i8> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int16MemoryStream = MappedStream<mode, i16, MappedMemory<i16> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int32MemoryStream = MappedStream<mode, i32, MappedMemory<i32> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int64MemoryStream = MappedStream<mode, i64, MappedMemory<i64> >;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using ByteMemoryStream = Uint8MemoryStream<mode>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using CharMemoryStream = Int8MemoryStream<mode>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint8FileStream = MappedStream<mode, u8, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint16FileStream = MappedStream<mode, u16, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint32FileStream = MappedStream<mode, u32, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Uint64FileStream = MappedStream<mode, u64, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int8FileStream = MappedStream<mode, i8, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int16FileStream = MappedStream<mode, i16, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int32FileStream = MappedStream<mode, i32, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using Int64FileStream = MappedStream<mode, i64, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using ByteFileStream = MappedStream<mode, u8, MappedFile>;

    template<auto mode> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly>
    using CharFileStream = MappedStream<mode, i8, MappedFile>;

    using Uint8ReadMemoryStream = Uint8MemoryStream<AccessMode::ReadOnly>;

    using Uint16ReadMemoryStream = Uint16MemoryStream<AccessMode::ReadOnly>;

    using Uint32ReadMemoryStream = Uint32MemoryStream<AccessMode::ReadOnly>;

    using Uint64ReadMemoryStream = Uint64MemoryStream<AccessMode::ReadOnly>;

    using Int8ReadMemoryStream = Int8MemoryStream<AccessMode::ReadOnly>;

    using Int16ReadMemoryStream = Int16MemoryStream<AccessMode::ReadOnly>;

    using Int32ReadMemoryStream = Int32MemoryStream<AccessMode::ReadOnly>;

    using Int64ReadMemoryStream = Int64MemoryStream<AccessMode::ReadOnly>;

    using ByteReadMemoryStream = ByteMemoryStream<AccessMode::ReadOnly>;

    using CharReadMemoryStream = CharMemoryStream<AccessMode::ReadOnly>;

    using Uint8WriteMemoryStream = Uint8MemoryStream<AccessMode::WriteOnly>;

    using Uint16WriteMemoryStream = Uint16MemoryStream<AccessMode::WriteOnly>;

    using Uint32WriteMemoryStream = Uint32MemoryStream<AccessMode::WriteOnly>;

    using Uint64WriteMemoryStream = Uint64MemoryStream<AccessMode::WriteOnly>;

    using Int8WriteMemoryStream = Int8MemoryStream<AccessMode::WriteOnly>;

    using Int16WriteMemoryStream = Int16MemoryStream<AccessMode::WriteOnly>;

    using Int32WriteMemoryStream = Int32MemoryStream<AccessMode::WriteOnly>;

    using Int64WriteMemoryStream = Int64MemoryStream<AccessMode::WriteOnly>;

    using ByteWriteMemoryStream = ByteMemoryStream<AccessMode::WriteOnly>;

    using CharWriteMemoryStream = CharMemoryStream<AccessMode::WriteOnly>;

    using Uint8ReadWriteMemoryStream = Uint8MemoryStream<AccessMode::ReadWrite>;

    using Uint16ReadWriteMemoryStream = Uint16MemoryStream<AccessMode::ReadWrite>;

    using Uint32ReadWriteMemoryStream = Uint32MemoryStream<AccessMode::ReadWrite>;

    using Uint64ReadWriteMemoryStream = Uint64MemoryStream<AccessMode::ReadWrite>;

    using Int8ReadWriteMemoryStream = Int8MemoryStream<AccessMode::ReadWrite>;

    using Int16ReadWriteMemoryStream = Int16MemoryStream<AccessMode::ReadWrite>;

    using Int32ReadWriteMemoryStream = Int32MemoryStream<AccessMode::ReadWrite>;

    using Int64ReadWriteMemoryStream = Int64MemoryStream<AccessMode::ReadWrite>;

    using ByteReadWriteMemoryStream = ByteMemoryStream<AccessMode::ReadWrite>;

    using CharReadWriteMemoryStream = CharMemoryStream<AccessMode::ReadWrite>;

    using Uint8ReadFileStream = Uint8FileStream<AccessMode::ReadOnly>;

    using Uint16ReadFileStream = Uint16FileStream<AccessMode::ReadOnly>;

    using Uint32ReadFileStream = Uint32FileStream<AccessMode::ReadOnly>;

    using Uint64ReadFileStream = Uint64FileStream<AccessMode::ReadOnly>;

    using Int8ReadFileStream = Int8FileStream<AccessMode::ReadOnly>;

    using Int16ReadFileStream = Int16FileStream<AccessMode::ReadOnly>;

    using Int32ReadFileStream = Int32FileStream<AccessMode::ReadOnly>;

    using Int64ReadFileStream = Int64FileStream<AccessMode::ReadOnly>;

    using ByteReadFileStream = ByteFileStream<AccessMode::ReadOnly>;

    using CharReadFileStream = CharFileStream<AccessMode::ReadOnly>;

    using Uint8WriteFileStream = Uint8FileStream<AccessMode::WriteOnly>;

    using Uint16WriteFileStream = Uint16FileStream<AccessMode::WriteOnly>;

    using Uint32WriteFileStream = Uint32FileStream<AccessMode::WriteOnly>;

    using Uint64WriteFileStream = Uint64FileStream<AccessMode::WriteOnly>;

    using Int8WriteFileStream = Int8FileStream<AccessMode::WriteOnly>;

    using Int16WriteFileStream = Int16FileStream<AccessMode::WriteOnly>;

    using Int32WriteFileStream = Int32FileStream<AccessMode::WriteOnly>;

    using Int64WriteFileStream = Int64FileStream<AccessMode::WriteOnly>;

    using ByteWriteFileStream = ByteFileStream<AccessMode::WriteOnly>;

    using CharWriteFileStream = CharFileStream<AccessMode::WriteOnly>;

    using Uint8ReadWriteFileStream = Uint8FileStream<AccessMode::ReadWrite>;

    using Uint16ReadWriteFileStream = Uint16FileStream<AccessMode::ReadWrite>;

    using Uint32ReadWriteFileStream = Uint32FileStream<AccessMode::ReadWrite>;

    using Uint64ReadWriteFileStream = Uint64FileStream<AccessMode::ReadWrite>;

    using Int8ReadWriteFileStream = Int8FileStream<AccessMode::ReadWrite>;

    using Int16ReadWriteFileStream = Int16FileStream<AccessMode::ReadWrite>;

    using Int32ReadWriteFileStream = Int32FileStream<AccessMode::ReadWrite>;

    using Int64ReadWriteFileStream = Int64FileStream<AccessMode::ReadWrite>;

    using ByteReadWriteFileStream = ByteFileStream<AccessMode::ReadWrite>;

    using CharReadWriteFileStream = CharFileStream<AccessMode::ReadWrite>;

    template<typename ByteStream>
    concept is_u32_read_stream = std::is_same_v<ByteStream, Uint32ReadMemoryStream> || std::is_same_v<ByteStream, Uint32ReadFileStream>;

    template<typename ByteStream>
    concept is_read_stream =
            std::is_same_v<ByteStream, Uint8ReadMemoryStream> ||
            std::is_same_v<ByteStream, Uint16ReadMemoryStream> ||
            std::is_same_v<ByteStream, Uint32ReadMemoryStream> ||
            std::is_same_v<ByteStream, Uint64ReadMemoryStream> ||
            std::is_same_v<ByteStream, Int8ReadMemoryStream> ||
            std::is_same_v<ByteStream, Int16ReadMemoryStream> ||
            std::is_same_v<ByteStream, Int32ReadMemoryStream> ||
            std::is_same_v<ByteStream, Int64ReadMemoryStream> ||
            std::is_same_v<ByteStream, ByteReadMemoryStream> ||
            std::is_same_v<ByteStream, CharReadMemoryStream> ||
            std::is_same_v<ByteStream, Uint8ReadFileStream> ||
            std::is_same_v<ByteStream, Uint16ReadFileStream> ||
            std::is_same_v<ByteStream, Uint32ReadFileStream> ||
            std::is_same_v<ByteStream, Uint64ReadFileStream> ||
            std::is_same_v<ByteStream, Int8ReadFileStream> ||
            std::is_same_v<ByteStream, Int16ReadFileStream> ||
            std::is_same_v<ByteStream, Int32ReadFileStream> ||
            std::is_same_v<ByteStream, Int64ReadFileStream> ||
            std::is_same_v<ByteStream, ByteReadFileStream> ||
            std::is_same_v<ByteStream, CharReadFileStream>;

    template<typename ByteStream>
    concept is_u32_write_stream = std::is_same_v<ByteStream, Uint32WriteMemoryStream> || std::is_same_v<ByteStream, Uint32WriteFileStream>;

    template<typename ByteStream>
    concept is_write_stream =
            std::is_same_v<ByteStream, Uint8WriteMemoryStream> ||
            std::is_same_v<ByteStream, Uint16WriteMemoryStream> ||
            std::is_same_v<ByteStream, Uint32WriteMemoryStream> ||
            std::is_same_v<ByteStream, Uint64WriteMemoryStream> ||
            std::is_same_v<ByteStream, Int8WriteMemoryStream> ||
            std::is_same_v<ByteStream, Int16WriteMemoryStream> ||
            std::is_same_v<ByteStream, Int32WriteMemoryStream> ||
            std::is_same_v<ByteStream, Int64WriteMemoryStream> ||
            std::is_same_v<ByteStream, ByteWriteMemoryStream> ||
            std::is_same_v<ByteStream, CharWriteMemoryStream> ||
            std::is_same_v<ByteStream, Uint8WriteFileStream> ||
            std::is_same_v<ByteStream, Uint16WriteFileStream> ||
            std::is_same_v<ByteStream, Uint32WriteFileStream> ||
            std::is_same_v<ByteStream, Uint64WriteFileStream> ||
            std::is_same_v<ByteStream, Int8WriteFileStream> ||
            std::is_same_v<ByteStream, Int16WriteFileStream> ||
            std::is_same_v<ByteStream, Int32WriteFileStream> ||
            std::is_same_v<ByteStream, Int64WriteFileStream> ||
            std::is_same_v<ByteStream, ByteWriteFileStream> ||
            std::is_same_v<ByteStream, CharWriteFileStream>;

}
