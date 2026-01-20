/* W - Weak function (defined) */
__attribute__((weak)) void weak_function(void) {
}

/* w - Weak function (undefined) */
__attribute__((weak)) extern void weak_undef_function(void);

int main(void) {
    weak_function();
    if (weak_undef_function)
        weak_undef_function();
    return 0;
}
