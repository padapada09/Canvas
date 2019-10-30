double min(double a, double b)
{
    if (a < b) return a;
    else return b;
}

double max(double a, double b)
{
    if (a < b) return b;
    else return a;
}

// LPVOID variable;
    // DWORD thread_id;
    // second_process = CreateThread(
    //     NULL,
    //     0,
    //     this->render,
    //     variable,
    //     0,
    //     &thread_id
    // );