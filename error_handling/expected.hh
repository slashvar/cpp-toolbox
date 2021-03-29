#pragma once
#include <functional>
#include <type_traits>
#include <variant>

template <typename Value, typename Error>
class Expected
{
public:
    bool has_value() const
    {
        return std::holds_alternative<Value>(content_);
    }

    template <typename Function>
    auto and_then(Function&& function) -> decltype(function(std::declval<Value>()))
    {
        return std::visit(
            overload_{ function,
                       [](const Error& e) -> decltype(function(std::declval<Value>())) {
                           return { e };
                       } },
            content_);
    }

    template <typename Function>
    auto map(Function&& op)
        -> Expected<std::decay_t<decltype(op(std::declval<Value>()))>, Error>
    {
        using Return = std::decay_t<decltype(op(std::declval<Value>()))>;
        return std::visit(
            overload_{ [&op](const Value& v) -> Expected<Return, Error> { return { op(v) }; },
                       [](const Error& e) -> Expected<Return, Error> { return { e }; } },
            content_);
    }

    const Value& get() const
    {
        if (has_value()) {
            return std::get<Value>(content_);
        }
        throw std::get<Error>(content_);
    }

    const Error& get_error() const
    {
        return std::get<Error>(content_);
    }

public:
    Expected(Value&& v) : content_(v) {}
    Expected(const Error& e) : content_(e) {}

private:
    std::variant<Value, Error> content_;

    template <typename... Ts>
    struct overload_ : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload_(Ts...) -> overload_<Ts...>;
};
