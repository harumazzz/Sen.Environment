// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <assert.h>
#include "./detail/mappedfile_common.hpp"
#include <windows.h>

// must come after windows.h
#include <subauth.h>

namespace decodeless {

namespace detail {

inline size_t pageSize() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize;
}

static inline size_t pageSizeCached() {
    static size_t size = pageSize();
    return size;
}

class Message {
public:
    Message() = delete;
    Message(const Message& other) = delete;
    Message& operator=(const Message& other) = delete;
    Message(DWORD error, HMODULE module = NULL)
        : m_size(FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                    FORMAT_MESSAGE_IGNORE_INSERTS |
                                    (module ? FORMAT_MESSAGE_FROM_HMODULE : 0),
                                module, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPSTR)&m_buffer, 0, NULL)) {}
    ~Message() { LocalFree(m_buffer); }
    std::string str() const { return {m_buffer, m_size}; }

private:
    LPSTR m_buffer;
    DWORD m_size;
};

class LastError : public mapping_error {
public:
    LastError()
        : mapping_error(Message(::GetLastError()).str()) {}
};

class LastMappedFileError : public mapped_file_error {
public:
    LastMappedFileError(const fs::path& context)
        : mapped_file_error(Message(::GetLastError()).str(), context,
                            std::error_code(::GetLastError(), std::system_category())) {}
};

class Handle {
public:
    Handle(HANDLE&& handle) noexcept
        : m_handle(handle) {}
    Handle() = delete;
    Handle(const Handle& other) = delete;
    Handle(Handle&& other) noexcept
        : m_handle(other.m_handle) {
        other.m_handle = INVALID_HANDLE_VALUE;
    };
    Handle& operator=(const Handle& other) = delete;
    Handle& operator=(Handle&& other) noexcept {
        if (m_handle != INVALID_HANDLE_VALUE)
            CloseHandle(m_handle);
        m_handle = other.m_handle;
        other.m_handle = INVALID_HANDLE_VALUE;
        return *this;
    };
    ~Handle() {
        if (m_handle != INVALID_HANDLE_VALUE)
            CloseHandle(m_handle);
    }
    operator const HANDLE&() const { return m_handle; }
    operator bool() const { return m_handle != INVALID_HANDLE_VALUE; }

private:
    HANDLE m_handle;
};

class FileHandle : public Handle {
public:
    template <class... Args>
    FileHandle(const fs::path& path, Args&&... args)
        : Handle(CreateFileW(path.c_str(), std::forward<Args>(args)...)) {
        if (!*this) {
            throw LastMappedFileError(path);
        }
    }
    void setPointer(ptrdiff_t distance, DWORD moveMethod = FILE_BEGIN) {
        SetFilePointerEx(*this, LARGE_INTEGER{.QuadPart = distance}, nullptr, moveMethod);
    }
    void   setEndOfFile() { SetEndOfFile(*this); }
    size_t size() {
        LARGE_INTEGER result;
        if (!GetFileSizeEx(*this, &result))
            throw LastError();
        return result.QuadPart;
    }
};

class FileMappingHandle : public Handle {
public:
    // Mapping backed by filesystem file
    FileMappingHandle(const FileHandle& file, LPSECURITY_ATTRIBUTES fileMappingAttributes,
                      DWORD protect, size_t maximumSize, LPCWSTR name = nullptr)
        : FileMappingHandle(static_cast<HANDLE>(file), fileMappingAttributes, protect, maximumSize,
                            name) {}

    // Mapping backed by system paging file
    FileMappingHandle(LPSECURITY_ATTRIBUTES fileMappingAttributes, DWORD protect,
                      size_t maximumSize, LPCWSTR name = nullptr)
        : FileMappingHandle(INVALID_HANDLE_VALUE, fileMappingAttributes, protect, maximumSize,
                            name) {}

private:
    FileMappingHandle(HANDLE fileHandle, LPSECURITY_ATTRIBUTES fileMappingAttributes, DWORD protect,
                      size_t maximumSize, LPCWSTR name = nullptr)
        : Handle(CreateFileMappingW(fileHandle, fileMappingAttributes, protect,
                                    (maximumSize >> 32) & 0xffffffff, maximumSize & 0xffffffff,
                                    name)) {
        if (!*this) {
            throw LastError();
        }
    }
};

class FileMappingView {
public:
    template <class... Args>
    FileMappingView(const FileMappingHandle& fileMapping, DWORD desiredAccess,
                    size_t fileOffset = 0, size_t bytesToMap = 0 /* 0 means to the end */,
                    void* baseAddress = nullptr)
        : m_address(MapViewOfFileEx(fileMapping, desiredAccess, (fileOffset >> 32) & 0xffffffff,
                                    fileOffset & 0xffffffff, bytesToMap, baseAddress)) {
        if (!m_address) {
            throw LastError();
        }
    }
    FileMappingView() = delete;
    FileMappingView(const FileMappingView& other) = delete;
    FileMappingView(FileMappingView&& other) noexcept { *this = std::move(other); }
    FileMappingView& operator=(const FileMappingView& other) = delete;
    FileMappingView& operator=(FileMappingView&& other) noexcept {
        m_address = other.m_address;
        other.m_address = nullptr;
        return *this;
    }
    ~FileMappingView() { UnmapViewOfFile(m_address); }
    void*                    address() const { return m_address; }
    MEMORY_BASIC_INFORMATION query() const {
        MEMORY_BASIC_INFORMATION result;
        (void)VirtualQuery(address(), &result, sizeof(result));
        return result;
    }

private:
    LPVOID m_address;
};

template <bool Writable>
class MappedFile {
public:
    using data_type = std::conditional_t<Writable, void*, const void*>;
    MappedFile(const fs::path& path)
        : m_file(path, GENERIC_READ | (Writable ? GENERIC_WRITE : 0),
                 FILE_SHARE_READ | (Writable ? FILE_SHARE_WRITE : 0), nullptr, OPEN_EXISTING,
                 FILE_ATTRIBUTE_NORMAL, nullptr)
        , m_size(m_file.size())
        , m_mapping(m_file, nullptr, PAGE_READONLY, m_size, nullptr)
        , m_rawView(m_mapping, FILE_MAP_READ) {}
    data_type data() const { return m_rawView.address(); }
    size_t    size() const { return m_size; }

private:
    FileHandle        m_file;
    size_t            m_size;
    FileMappingHandle m_mapping;
    FileMappingView   m_rawView;
};

class DynamicLibrary {
public:
    DynamicLibrary() = delete;
    DynamicLibrary(const DynamicLibrary& other) = delete;
    DynamicLibrary(DynamicLibrary&& other) noexcept
        : m_module(other.m_module) {
        other.m_module = nullptr;
    };
    DynamicLibrary(const fs::path& path)
        : m_module(::LoadLibraryW(path.c_str())) {
        if (!m_module) {
            throw LastError(); //("Failed to load " + path.string());
        }
    }
    DynamicLibrary& operator=(const DynamicLibrary& other) = delete;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept {
        if (m_module)
            FreeLibrary(m_module);
        m_module = other.m_module;
        other.m_module = nullptr;
    }
    ~DynamicLibrary() {
        if (m_module)
            FreeLibrary(m_module);
    }
    operator HMODULE() const { return m_module; }

    template <typename FuncType>
    FuncType* get(const std::string& functionName) const {
        FARPROC functionAddress = ::GetProcAddress(m_module, functionName.c_str());
        if (!functionAddress) {
            throw LastError(); //("Failed to get address for " + functionName);
        }
        return reinterpret_cast<FuncType*>(functionAddress);
    }

private:
    HMODULE m_module;
};

class Env {
public:
    explicit Env(const std::string& name) {
        m_value.resize(GetEnvironmentVariableA(name.c_str(), nullptr, 0));
        m_value.resize(GetEnvironmentVariableA(name.c_str(), m_value.data(),
                                               static_cast<DWORD>(m_value.size())));
    }
    operator const std::string&() const { return m_value; }

private:
    std::string m_value;
};

// DANGER: copy/pasting from e.g.
// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntcreatesection and
// http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FSection%2FNtMapViewOfSection.html
// TODO: make WDK a dependency? ugh..
// *dumpbin.exe /EXPORTS Windows/System32/ntdll.dll to verify symbols exist

using NTSTATUS = LONG;

typedef enum _SECTION_INFORMATION_CLASS {
    SectionBasicInformation,
    SectionImageInformation
} SECTION_INFORMATION_CLASS,
    *PSECTION_INFORMATION_CLASS;

typedef enum _SECTION_INHERIT { ViewShare = 1, ViewUnmap = 2 } SECTION_INHERIT, *PSECTION_INHERIT;

typedef struct {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _SECTION_BASIC_INFORMATION {
    ULONG         Unknown;
    ULONG         SectionAttributes;
    LARGE_INTEGER SectionSize;
} SECTION_BASIC_INFORMATION, *PSECTION_BASIC_INFORMATION;

typedef struct _SECTION_IMAGE_INFORMATION {
    PVOID EntryPoint;
    ULONG StackZeroBits;
    ULONG StackReserved;
    ULONG StackCommit;
    ULONG ImageSubsystem;
    WORD  SubSystemVersionLow;
    WORD  SubSystemVersionHigh;
    ULONG Unknown1;
    ULONG ImageCharacteristics;
    ULONG ImageMachineType;
    ULONG Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

using NtAllocateVirtualMemoryType = NTSTATUS(HANDLE ProcessHandle, PVOID* BaseAddress,
                                             ULONG_PTR ZeroBits, PSIZE_T RegionSize,
                                             ULONG AllocationType, ULONG Protect);
using NtCreateSectionType = NTSTATUS(PHANDLE SectionHandle, ACCESS_MASK DesiredAccess,
                                     POBJECT_ATTRIBUTES ObjectAttributes,
                                     PLARGE_INTEGER MaximumSize, ULONG SectionPageProtection,
                                     ULONG AllocationAttributes, HANDLE FileHandle);
using NtExtendSectionType = NTSTATUS(HANDLE SectionHandle, PLARGE_INTEGER NewSectionSize);
using NtFreeVirtualMemoryType = NTSTATUS(HANDLE ProcessHandle, PVOID* BaseAddress,
                                         PSIZE_T RegionSize, ULONG FreeType);
using NtMapViewOfSectionType = NTSTATUS(HANDLE SectionHandle, HANDLE ProcessHandle,
                                        PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize,
                                        PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize,
                                        SECTION_INHERIT InheritDisposition, ULONG AllocationType,
                                        ULONG Win32Protect);
using NtOpenSectionType = NTSTATUS(PHANDLE SectionHandle, ACCESS_MASK DesiredAccess,
                                   POBJECT_ATTRIBUTES ObjectAttributes);
using NtCloseType = NTSTATUS(HANDLE Handle);
using NtQuerySectionType = NTSTATUS(HANDLE                    SectionHandle,
                                    SECTION_INFORMATION_CLASS InformationClass,
                                    PVOID InformationBuffer, ULONG InformationBufferSize,
                                    PULONG ResultLength);
using NtUnmapViewOfSectionType = NTSTATUS(HANDLE ProcessHandle, PVOID BaseAddress);

class NtStatusError : public mapping_error {
public:
    NtStatusError(const DynamicLibrary& dll, const std::string& context, NTSTATUS status)
        : mapping_error(context + ": " + Message(status, dll).str()) {}
};

class NtifsSection {
public:
    NtifsSection()
        : m_ntdll("ntdll.dll")
        , m_NtAllocateVirtualMemory(
              m_ntdll.get<NtAllocateVirtualMemoryType>("NtAllocateVirtualMemory"))
        , m_NtCreateSection(m_ntdll.get<NtCreateSectionType>("NtCreateSection"))
        , m_NtExtendSection(m_ntdll.get<NtExtendSectionType>("NtExtendSection"))
        , m_NtFreeVirtualMemory(m_ntdll.get<NtFreeVirtualMemoryType>("NtFreeVirtualMemory"))
        , m_NtMapViewOfSection(m_ntdll.get<NtMapViewOfSectionType>("NtMapViewOfSection"))
        , m_NtOpenSection(m_ntdll.get<NtOpenSectionType>("NtOpenSection"))
        , m_NtClose(m_ntdll.get<NtCloseType>("NtClose"))
        , m_NtQuerySection(m_ntdll.get<NtQuerySectionType>("NtQuerySection"))
        , m_NtUnmapViewOfSection(m_ntdll.get<NtUnmapViewOfSectionType>("NtUnmapViewOfSection")) {}

    static HANDLE         CurrentProcess() { return reinterpret_cast<HANDLE>((LONG_PTR)-1); }
    const DynamicLibrary& ntdll() const { return m_ntdll; }

private:
    DynamicLibrary m_ntdll;

public:
    NtAllocateVirtualMemoryType* const m_NtAllocateVirtualMemory;
    NtCreateSectionType* const         m_NtCreateSection;
    NtExtendSectionType* const         m_NtExtendSection;
    NtFreeVirtualMemoryType* const     m_NtFreeVirtualMemory;
    NtMapViewOfSectionType* const      m_NtMapViewOfSection;
    NtOpenSectionType* const           m_NtOpenSection;
    NtCloseType* const                 m_NtClose;
    NtQuerySectionType* const          m_NtQuerySection;
    NtUnmapViewOfSectionType* const    m_NtUnmapViewOfSection;
};

// It is cumbersome to pass around a module handle, so this static call returns a common one when
// needed. Ideally this would be in just one compilation unit but this is a header-only library.
static inline NtifsSection& ntifs() {
    static NtifsSection ntifs;
    return ntifs;
}

class VirtualMemory {
public:
    VirtualMemory() = delete;
    VirtualMemory(const VirtualMemory& other) = delete;
    VirtualMemory(const NtifsSection& dll, HANDLE ProcessHandle, ULONG_PTR ZeroBits,
                  SIZE_T RegionSize, ULONG AllocationType, ULONG Protect)
        : m_dll(dll)
        , m_process(ProcessHandle)
        , m_address(allocateVirtualMemory(dll, m_process, ZeroBits, nullptr, RegionSize,
                                          AllocationType, Protect)
                        .first) {}
    constexpr VirtualMemory(VirtualMemory&& other) noexcept
        : m_dll(other.m_dll)
        , m_address(other.m_address) {
        other.m_address = nullptr;
    }
    VirtualMemory& operator=(const VirtualMemory& other) = delete;
    VirtualMemory& operator=(VirtualMemory&& other) noexcept {
        free();
        m_address = other.m_address;
        other.m_address = nullptr;
        return *this;
    }
    ~VirtualMemory() { free(); }

    // Use to convert a region of memory from reserved to committed. Returns the possibly-rounded-up
    // size of the modified region.
    size_t commit(ULONG_PTR ZeroBits, SIZE_T Offset, SIZE_T RegionSize, ULONG Protect) {
        assert(RegionSize != 0);
        auto [addr, size] = allocateVirtualMemory(m_dll, m_process, ZeroBits, offsetAddress(Offset),
                                                  RegionSize, MEM_COMMIT, Protect);
        assert(Offset > 0 || addr == m_address);
        return size;
    }

    // Use to convert a committed region of memory to a reserved virtual address range.
    void decommit(SIZE_T Offset, SIZE_T RegionSize) {
        // Round up offset to the next page
        size_t ps = pageSizeCached();
        size_t offsetRoundedUp = ((Offset + ps - 1) / ps) * ps;
        size_t end = Offset + RegionSize;

        // Ignore degenerate decommits where the region decommitted does not cross a page boundary
        if (end <= offsetRoundedUp)
            return;

        void*    address = offsetAddress(offsetRoundedUp);
        size_t   size = end - offsetRoundedUp;
        NTSTATUS status = m_dll.m_NtFreeVirtualMemory(m_process, &address, &size, MEM_DECOMMIT);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(m_dll.ntdll(), "NtFreeVirtualMemory", status);
    }
    void* address() const { return m_address; }

private:
    static std::pair<void*, size_t> allocateVirtualMemory(const NtifsSection& dll,
                                                          HANDLE ProcessHandle, ULONG_PTR ZeroBits,
                                                          PVOID BaseAddress, SIZE_T RegionSize,
                                                          ULONG AllocationType, ULONG Protect) {
        void*    result = BaseAddress;
        size_t   resultSize = RegionSize;
        NTSTATUS status = dll.m_NtAllocateVirtualMemory(ProcessHandle, &result, ZeroBits,
                                                        &resultSize, AllocationType, Protect);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(dll.ntdll(), "NtAllocateVirtualMemory", status);
        return {result, resultSize};
    }
    void free() {
        if (m_address) {
            size_t   size = 0;
            NTSTATUS status =
                m_dll.m_NtFreeVirtualMemory(m_process, &m_address, &size, MEM_RELEASE);
            if (status != STATUS_SUCCESS)
                NtStatusError(m_dll.ntdll(), "NtFreeVirtualMemory", status)
                    .print(); // can't throw from destructor. ignore the error
        }
    }
    void* offsetAddress(SIZE_T Offset) const {
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_address) + Offset);
    }
    const NtifsSection& m_dll;
    HANDLE              m_process = INVALID_HANDLE_VALUE;
    void*               m_address = nullptr;
};

template <ULONG SectionPageProtection>
class Section : public Handle {
public:
    Section(Section&& other) noexcept = default;
    Section(const NtifsSection& dll, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,
            size_t MaximumSize, ULONG AllocationAttributes, HANDLE FileHandle)
        : Handle(createSection(dll, DesiredAccess, ObjectAttributes, MaximumSize,
                               SectionPageProtection, AllocationAttributes, FileHandle))
        , m_dll(dll)
        , m_size(MaximumSize) {}
    Section& operator=(Section&& other) noexcept {
        Handle::operator=(std::move(other));
        m_size = other.m_size;
        return *this;
    }

    size_t size() const { return m_size; }

    LONGLONG extend(size_t NewSectionSize) {
        // Truncation will do nothing and is not supported.
        // https://stackoverflow.com/questions/51311421/memory-mapped-file-truncation-on-windows
        LARGE_INTEGER NewSectionSizeL{.QuadPart = LONGLONG(NewSectionSize)};
        NTSTATUS      status = m_dll.m_NtExtendSection(*this, &NewSectionSizeL);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(m_dll.ntdll(), "NtExtendSection", status);
        m_size = NewSectionSize;
        return NewSectionSizeL.QuadPart;
    }

    SECTION_BASIC_INFORMATION queryBasic() const {
        SECTION_BASIC_INFORMATION result;
        ULONG                     written{};
        NTSTATUS status = m_dll.m_NtQuerySection(*this, SectionBasicInformation, &result,
                                                 sizeof(result), &written);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(m_dll.ntdll(), "NtQuerySection", status);
        return result;
    };

    SECTION_IMAGE_INFORMATION queryImage() const {
        SECTION_IMAGE_INFORMATION result;
        ULONG                     written{};
        NTSTATUS status = m_dll.m_NtQuerySection(*this, SectionImageInformation, &result,
                                                 sizeof(result), &written);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(m_dll.ntdll(), "NtQuerySection", status);
        return result;
    };

private:
    static HANDLE createSection(const NtifsSection& dll, ACCESS_MASK DesiredAccess,
                                POBJECT_ATTRIBUTES ObjectAttributes, LONGLONG MaximumSize,
                                ULONG SectionPageProtection, ULONG AllocationAttributes,
                                HANDLE FileHandle) {
        HANDLE        result;
        LARGE_INTEGER MaximumSizeL{.QuadPart = MaximumSize};
        NTSTATUS      status =
            dll.m_NtCreateSection(&result, DesiredAccess, ObjectAttributes, &MaximumSizeL,
                                  SectionPageProtection, AllocationAttributes, FileHandle);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(dll.ntdll(), "NtCreateSection", status);
        return result;
    }
    const NtifsSection& m_dll;
    size_t              m_size;
};

static_assert(std::is_move_constructible_v<Section<PAGE_READWRITE>>);
static_assert(std::is_move_assignable_v<Section<PAGE_READWRITE>>);

template <ULONG SectionPageProtection>
class SectionView {
public:
    static constexpr bool writable =
        (SectionPageProtection &
         (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0;
    using address_type = std::conditional_t<writable, void*, const void*>;
    SectionView() = delete;
    SectionView(const SectionView& other) = delete;
    SectionView(const NtifsSection& dll, const Section<SectionPageProtection>& Section,
                HANDLE ProcessHandle, ULONG_PTR ZeroBits, size_t CommitSize, size_t SectionOffset,
                size_t ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType)
        : m_dll(dll)
        , m_process(ProcessHandle)
        , m_address(mapViewOfSection(dll, Section, ProcessHandle, ZeroBits, CommitSize,
                                     SectionOffset, ViewSize, InheritDisposition, AllocationType,
                                     SectionPageProtection)) {}
    ~SectionView() {
        if (m_address)
            unmap();
    }
    SectionView(SectionView&& other) noexcept
        : m_dll(other.m_dll)
        , m_process(other.m_process)
        , m_address(other.m_address) {
        other.m_address = nullptr;
    }
    SectionView& operator=(const SectionView& other) = delete;
    SectionView& operator=(SectionView&& other) noexcept {
        if (m_address)
            unmap();
        m_process = other.m_process;
        m_address = other.m_address;
        other.m_address = nullptr;
        return *this;
    };
    address_type             address() const { return m_address; }
    MEMORY_BASIC_INFORMATION query(ptrdiff_t offset = 0) const {
        MEMORY_BASIC_INFORMATION result;
        (void)VirtualQuery(reinterpret_cast<std::byte*>(address()) + offset, &result,
                           sizeof(result));
        return result;
    }

private:
    static void* mapViewOfSection(const NtifsSection& dll, HANDLE SectionHandle,
                                  HANDLE ProcessHandle, ULONG_PTR ZeroBits, size_t CommitSize,
                                  size_t SectionOffset, size_t ViewSize,
                                  SECTION_INHERIT InheritDisposition, ULONG AllocationType,
                                  ULONG Win32Protect) {
        void*         result = nullptr;
        LARGE_INTEGER SectionOffsetL{.QuadPart = LONGLONG(SectionOffset)};
        SIZE_T        ViewSizeL{ViewSize};
        NTSTATUS      status = dll.m_NtMapViewOfSection(
            SectionHandle, ProcessHandle, &result, ZeroBits, CommitSize, &SectionOffsetL,
            &ViewSizeL, InheritDisposition, AllocationType, Win32Protect);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(dll.ntdll(), "NtMapViewOfSection", status);
        return result;
    }
    void unmap() {
        NTSTATUS status = m_dll.m_NtUnmapViewOfSection(m_process, m_address);
        if (status != STATUS_SUCCESS)
            throw NtStatusError(m_dll.ntdll(), "NtUnmapViewOfSection", status);
    }

    const NtifsSection& m_dll;
    HANDLE              m_process;
    address_type        m_address;
};

static_assert(std::is_move_constructible_v<SectionView<PAGE_READWRITE>>);
static_assert(std::is_move_assignable_v<SectionView<PAGE_READWRITE>>);

class ResizableMappedFile {
public:
    ResizableMappedFile(const fs::path& path, size_t maxSize)
        : m_capacity(maxSize)
        , m_file(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                 OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr) {
        size_t existingSize = m_file.size();
        if (existingSize > 0)
            resize(existingSize);
    }
    ~ResizableMappedFile() {
        // Truncate the file to the last size requested.
        if (m_file) {
            size_t finalSize = size();

            // Unmap the file before truncating the file
            m_view.reset();
            m_section.reset();

            // Truncate
            m_file.setPointer(finalSize);
            m_file.setEndOfFile();
        }
    }
    ResizableMappedFile(ResizableMappedFile&& other) = default;
    ResizableMappedFile& operator=(ResizableMappedFile&& other) = default;
    void*  data() const { return m_view ? m_view->address() : nullptr; }
    size_t size() const { return m_section ? m_section->size() : 0; }
    size_t capacity() const { return m_capacity; }
    void   resize(size_t size) {
        // Artificially fail on overflow. This seems to "just work" for windows,
        // but is probably unpredictable if there is no address space to expand.
        if (size > m_capacity)
            throw std::bad_alloc();

        // Note: truncation is ignored until the object is destroyed
        if (m_section) {
            m_section->extend(size);
        } else {
            m_section.emplace(ntifs(), SECTION_MAP_WRITE | SECTION_MAP_READ | SECTION_EXTEND_SIZE,
                                nullptr, size, SEC_COMMIT, m_file);
            m_view.emplace(ntifs(), *m_section, NtifsSection::CurrentProcess(), 0, 0, 0, m_capacity,
                             ViewUnmap, MEM_RESERVE);
        }
    }

private:
    size_t                                     m_capacity = 0;
    FileHandle                                 m_file;
    std::optional<Section<PAGE_READWRITE>>     m_section;
    std::optional<SectionView<PAGE_READWRITE>> m_view;
};

static_assert(std::is_move_constructible_v<ResizableMappedFile>);
static_assert(std::is_move_assignable_v<ResizableMappedFile>);

class ResizableMappedMemory {
public:
    ResizableMappedMemory(size_t initialSize, size_t maxSize)
        : m_capacity(maxSize)
        , m_memory(ntifs(), NtifsSection::CurrentProcess(), 0, m_capacity, MEM_RESERVE,
                   PAGE_NOACCESS) {
        if (initialSize)
            resize(initialSize);
    }
    void*  data() const { return m_size ? m_memory.address() : nullptr; }
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    void   resize(size_t size) {
        // Artificially fail on overflow. The NtAllocateVirtualMemory() call succeeds without error,
        // but the memory becomes unreadable afterwards.
        if (size > m_capacity)
            throw std::bad_alloc();

        if (size > m_size)
            m_memory.commit(0, 0, size, PAGE_READWRITE);
        else
            m_memory.decommit(size, m_size - size);
        m_size = size;
    }

private:
    static NtifsSection& ntifs() {
        static NtifsSection ntifs;
        return ntifs;
    }
    size_t        m_capacity = 0;
    size_t        m_size = 0;
    VirtualMemory m_memory;
};

static_assert(std::is_move_constructible_v<ResizableMappedMemory>);
static_assert(std::is_move_assignable_v<ResizableMappedMemory>);

} // namespace detail

} // namespace decodeless
