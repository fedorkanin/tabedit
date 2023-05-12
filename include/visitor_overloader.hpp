#pragma once

template <class... Ts>
struct VisitorOverloader : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
VisitorOverloader(Ts...) -> VisitorOverloader<Ts...>;
