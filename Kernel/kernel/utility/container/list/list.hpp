#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/container/list/common.hpp"

namespace Sen::Kernel {
    template<typename T>
    class CList;

    template<typename T>
    using List = CList<T>;

    template<typename T>
    class CList : public Common, public BaseContainer<T> {

    protected:

        using Common = Common;

        using Alloc = Allocator<T>;

    public:
        using Size = std::size_t;

        template<typename U>
        using Pointer = U *;

    protected:
        Size _capacity{};

    public:
        constexpr explicit CList(const Size& size)
            : BaseContainer<T>{Alloc::allocate(size), 0_size}, _capacity{size} {
        }

        constexpr explicit CList(Pointer<T> const& source, const Size& size)
            : BaseContainer<T>{source, size}, _capacity{size} {
        }

        constexpr explicit CList()
            : BaseContainer<T>{Alloc::allocate(64_size), 0_size}, _capacity{64_size} {
        }

        explicit constexpr CList(const std::span<T>& init)
            : BaseContainer<T>{Alloc::allocate(init.size()), init.size()}, _capacity{init.size()} {
            Alloc::copy(thiz.value, init.data(), init.size());
        }

        constexpr auto release() -> std::tuple<Pointer<T>, Size> override {
            auto raw = thiz.value;
            auto size = thiz._size;
            thiz.value = nullptr;
            thiz._size = 0_size;
            thiz._capacity = 0_size;
            return std::make_tuple(raw, size);
        }

        constexpr auto allocate(const Size& size) -> void {
            if (thiz.value != nullptr) {
                Alloc::deallocate(thiz.value);
            }
            thiz.value = Alloc::allocate(size);
            thiz._size = 0_size;
            thiz._capacity = size;
        }

        constexpr ~CList() override {
            if (thiz.value != nullptr) {
                Alloc::deallocate(thiz.value);
                thiz.value = nullptr;
            }
        }

        constexpr CList(const CList& other) : CList<T>{other._capacity} {
            Alloc::copy(thiz.value, other.value, other._size);
            thiz._size = other._size;
        }

        constexpr auto operator=(const CList& other) -> CList& {
            if (this != &other) {
                allocate(other._capacity);
                Alloc::copy(thiz.value, other.value, other._size);
                thiz._size = other._size;
            }
            return thiz;
        }

        constexpr auto operator[](const Size& index) const -> T& {
            assert_index(index < thiz._size, "Index out of bounds of the List", fmt::format("operator[]({})", index));
            return thiz.value[index];
        }

        constexpr CList(CList&& other) noexcept
            : BaseContainer<T>{std::move(other)}, _capacity{other._capacity} {
            other._capacity = 0;
        }

        constexpr auto operator=(CList&& other) noexcept -> CList& {
            if (this != &other) {
                Alloc::deallocate(thiz.value);
                thiz._size = other._size;
                thiz.value = other.value;
                thiz._capacity = other._capacity;
                other.value = nullptr;
                other._size = 0;
                other._capacity = 0;
            }
            return thiz;
        }

        constexpr auto reallocate(const Size& size) -> void {
            if (thiz._capacity < size) {
                auto new_value = Alloc::allocate(size);
                Alloc::move(new_value, thiz.value, thiz._size);
                Alloc::deallocate(thiz.value);
                thiz.value = new_value;
                thiz._capacity = size;
            }
        }

        template<typename U>
        constexpr auto append(U&& value) -> void {
            if (thiz._size + 1 > thiz._capacity) {
                reallocate(thiz._capacity * 2);
            }
            new (&thiz.value[thiz._size]) T(std::forward<U>(value));
            ++thiz._size;
        }

		friend class CArray<T>;

        constexpr auto append(
        ) -> void {
            if (thiz._size + 1 > thiz._capacity) {
                thiz.reallocate(thiz._capacity * 2);
            }
            ++thiz._size;
        }

        constexpr auto pop() -> void {
            assert_conditional(thiz._size > 0, "List is empty, cannot pop any element", "pop");
            --thiz._size;
        }

        constexpr auto pop(const Size& index) -> void {
            assert_conditional(index < thiz._size, "Index out of bounds of the List", "pop");
            Alloc::move(thiz.value + index, thiz.value + index + 1, thiz._size - index - 1);
            --thiz._size;
        }

        constexpr auto view() -> CListView<T> {
            return CListView<T>{thiz.value, thiz._size, thiz._capacity};
        }

        constexpr operator CListView<T>() {
            return CListView<T>{thiz.value, thiz._size, thiz._capacity};
        }

        constexpr auto insert(const Size& index, T&& element) -> void {
            assert_conditional(index <= thiz._size, "Index out of bounds of the List", "insert");
            if (thiz._size >= thiz._capacity) {
                reallocate(_capacity * 2);
            }
            Alloc::move(thiz.value + index + 1, thiz.value + index, thiz._size - index);
            thiz.value[index] = std::move(element);
            ++thiz._size;
        }

        constexpr static auto make_list(const std::span<T>& init) -> CList {
            return CList{init};
        }

        constexpr static auto make_list(const std::initializer_list<T>& init) -> CList {
            auto result = CList{init.size()};
            std::move(init.begin(), init.end(), result.value);
            result._size = init.size();
            return result;
        }

        constexpr auto capacity(
        ) -> Size {
            return thiz._capacity;
        }

        constexpr auto clear (
        ) -> void {
            Alloc::deallocate(thiz.value);
            thiz.value = nullptr;
            thiz._size = 0_size;
            thiz._capacity = 0_size;
        }

        constexpr auto assign(
            CArray<T> &other
        ) -> void {
            if (thiz.value != nullptr) {
                Alloc::deallocate(thiz.value);
            }
            thiz.value = other.value;
            thiz._size = other._size;
            thiz._capacity = other._size;
            other.value = nullptr;
            other._size = 0;
        }

        constexpr auto resize(
            const Size &new_size
        ) -> void {
            assert_conditional(new_size <= thiz._capacity, "Size must be smaller than current capacity", "size");
            thiz._size = new_size;
        }

        [[nodiscard]] constexpr auto size(

        ) const -> Size override {
            return thiz._size;
        }

        [[nodiscard]] constexpr auto is_empty() const -> bool {
            return thiz._size == 0;
        }

        [[nodiscard]] constexpr auto is_not_empty() const -> bool {
            return thiz._size != 0;
        }

        [[nodiscard]] static constexpr auto sizeof_value() -> Size {
            return sizeof(T);
        }

        constexpr auto assign(
            CList &other
        ) -> void {
            if (thiz.value != nullptr) {
                Alloc::deallocate(thiz.value);
            }
            thiz.value = other.value;
            thiz._size = other._size;
            thiz._capacity = other._size;
            other.value = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
    };


    template<typename T>
    inline auto operator<<(
        std::ostream &os,
        const CList<T> &array
    ) -> std::ostream & {
        os << "[";
        for (auto index: Range{array.size()}) {
            os << array[index];
            if (index < array.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template<typename T>
    struct is_list<CList<T> > : std::true_type {
    };

    template<typename T, typename... Args>
    inline auto make_list(
        Args &&... args
    ) -> List<T> {
        auto result = List<T>{sizeof...(args)};
        (result.append(std::forward<Args>(args)), ...);
        return result;
    }
}

namespace jsoncons {
    template<typename Json, typename T>
    struct json_type_traits<Json, Sen::Kernel::CList<T> > {
        using class_type = Sen::Kernel::CList<T>;
        using allocator_type = typename Json::allocator_type;

        static bool is(const Json &ajson) noexcept {
            return ajson.is_array();
        }

        static auto as(const Json &ajson) -> class_type {
            if (!is(ajson)) throw conv_error(conv_errc::conversion_failed, "Not an Array");

            class_type class_instance;
            for (const auto &item: ajson.array_range()) {
                class_instance.append(item.template as<T>());
            }
            return class_instance;
        }

        static Json to_json(const class_type &class_instance, allocator_type alloc = allocator_type()) {
            Json ajson(json_array_arg, alloc);
            for (const auto &item: class_instance) {
                ajson.push_back(item);
            }
            return ajson;
        }
    };

    template<typename T>
    struct is_json_type_traits_declared<Sen::Kernel::CList<T> > : public std::true_type {
    };
}
