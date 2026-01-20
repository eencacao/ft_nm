/* B - Global BSS (uninitialized) */
int global_bss;

/* b - Local BSS (static uninitialized) */
static int local_bss;

int main(void) {
    global_bss = 1;
    local_bss = 2;
    return global_bss + local_bss;
}
