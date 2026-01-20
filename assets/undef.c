/* U - Undefined symbol */
extern int undefined_external;
extern void undefined_function(void);

int main(void) {
    undefined_function();
    return undefined_external;
}
