/* D - Global data (initialized) */
int global_data = 42;

/* d - Local data (static initialized) */
static int local_data = 10;

int main(void) {
    return local_data + global_data;
}
