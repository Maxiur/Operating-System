#ifndef CHECKERROR_H
#define CHECKERROR_H

/*
    =======================================================
    Makro CheckError: wskazuje lokalizację błędu, wypisuje nazwę
    przekazywanego argumentu (#Arg) i kończy program.
    =======================================================
*/
#define CheckError(Arg)                                                              \
    if (!(Arg)) {                                                                     \
        fprintf(stderr, "[ERROR] %s:%d (%s) -> CheckError argument: %s\n",             \
        __FILE__, __LINE__, __func__, #Arg);                                            \
        exit(EXIT_FAILURE);                                                              \
}                                                                                         \

#endif //CHECKERROR_H
