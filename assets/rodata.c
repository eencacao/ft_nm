/* R - Global read-only data */
const int global_const = 100;

/* r - Local read-only data */
static const int local_const = 50;

/* String literal in rodata */
const char *global_string = "hello world";

int main(void) {
    return global_const + local_const;
}
