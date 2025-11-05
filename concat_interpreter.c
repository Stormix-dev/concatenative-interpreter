/*
 * ============================================================================
 * STACK-BASED CONCATENATIVE LANGUAGE INTERPRETER
 * INTERPRETE PER LINGUAGGIO CONCATENATIVO BASATO SU STACK
 * ============================================================================
 * 
 * A minimal but functional concatenative language interpreter written in C.
 * Un interprete minimale ma funzionale per linguaggi concatenativi scritto in C.
 * 
 * Author: [Gianmarco Simeoni]
 * License: MIT
 * ============================================================================
 */

#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <ctype.h>   

/* 
 * MAX_STACK: Maximum number of elements the stack can hold
 * MAX_STACK: Numero massimo di elementi che lo stack può contenere
 */
#define MAX_STACK 256

/* 
 * MAX_TOKEN: Maximum length of a single token (command or number)
 * MAX_TOKEN: Lunghezza massima di un singolo token (comando o numero)
 */
#define MAX_TOKEN 64

/*
 * ============================================================================
 * STACK DATA STRUCTURE | STRUTTURA DATI STACK
 * ============================================================================
 * 
 * The core data structure: a LIFO (Last In First Out) stack
 * La struttura dati principale: uno stack LIFO (Last In First Out)
 */
typedef struct {
    int data[MAX_STACK];  // Array to store stack elements | Array per memorizzare gli elementi
    int top;              // Index of top element (-1 if empty) | Indice dell'elemento in cima (-1 se vuoto)
} Stack;

/*
 * stack_init: Initialize an empty stack
 * stack_init: Inizializza uno stack vuoto
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack to initialize | puntatore allo stack da inizializzare
 */
void stack_init(Stack *s) {
    s->top = -1;  // Set top to -1 to indicate empty stack | Imposta top a -1 per indicare stack vuoto
}

/*
 * stack_is_empty: Check if the stack is empty
 * stack_is_empty: Controlla se lo stack è vuoto
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 * 
 * Returns | Ritorna:
 *   1 if empty, 0 otherwise | 1 se vuoto, 0 altrimenti
 */
int stack_is_empty(Stack *s) {
    return s->top == -1;  // Empty when top is -1 | Vuoto quando top è -1
}

/*
 * stack_is_full: Check if the stack is full
 * stack_is_full: Controlla se lo stack è pieno
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 * 
 * Returns | Ritorna:
 *   1 if full, 0 otherwise | 1 se pieno, 0 altrimenti
 */
int stack_is_full(Stack *s) {
    // Full when top reaches MAX_STACK - 1 | Pieno quando top raggiunge MAX_STACK - 1
    return s->top >= MAX_STACK - 1;
}

/*
 * stack_push: Push a value onto the stack
 * stack_push: Inserisce un valore sullo stack
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 *   value - integer value to push | valore intero da inserire
 * 
 * Note: Exits program if stack overflow occurs
 * Nota: Termina il programma se si verifica overflow
 */
void stack_push(Stack *s, int value) {
    // Check for overflow before pushing | Controlla overflow prima di inserire
    if (stack_is_full(s)) {
        fprintf(stderr, "Errore: stack overflow\n");
        exit(1);  // Exit with error code | Esce con codice di errore
    }
    // Pre-increment top, then store value | Pre-incrementa top, poi memorizza il valore
    s->data[++(s->top)] = value;
}

/*
 * stack_pop: Remove and return the top value from the stack
 * stack_pop: Rimuove e ritorna il valore in cima allo stack
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 * 
 * Returns | Ritorna:
 *   The top value | Il valore in cima
 * 
 * Note: Exits program if stack underflow occurs
 * Nota: Termina il programma se si verifica underflow
 */
int stack_pop(Stack *s) {
    // Check for underflow before popping | Controlla underflow prima di rimuovere
    if (stack_is_empty(s)) {
        fprintf(stderr, "Errore: stack underflow\n");
        exit(1);  // Exit with error code | Esce con codice di errore
    }
    // Return value at top, then post-decrement top | Ritorna valore in cima, poi post-decrementa top
    return s->data[(s->top)--];
}

/*
 * stack_peek: Return the top value without removing it
 * stack_peek: Ritorna il valore in cima senza rimuoverlo
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 * 
 * Returns | Ritorna:
 *   The top value | Il valore in cima
 */
int stack_peek(Stack *s) {
    // Check if stack is empty | Controlla se lo stack è vuoto
    if (stack_is_empty(s)) {
        fprintf(stderr, "Errore: stack vuoto\n");
        exit(1);
    }
    // Return top value without modifying stack | Ritorna valore in cima senza modificare lo stack
    return s->data[s->top];
}

/*
 * print_stack: Display all elements currently in the stack
 * print_stack: Visualizza tutti gli elementi attualmente nello stack
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 */
void print_stack(Stack *s) {
    printf("Stack: [ ");
    // Iterate from bottom to top | Itera dal fondo alla cima
    for (int i = 0; i <= s->top; i++) {
        printf("%d ", s->data[i]);  // Print each element | Stampa ogni elemento
    }
    printf("]\n");
}

/*
 * is_number: Check if a string represents a valid integer
 * is_number: Controlla se una stringa rappresenta un intero valido
 * 
 * Parameters | Parametri:
 *   str - string to check | stringa da controllare
 * 
 * Returns | Ritorna:
 *   1 if valid number, 0 otherwise | 1 se numero valido, 0 altrimenti
 */
int is_number(const char *str) {
    // Handle optional sign at the beginning | Gestisce segno opzionale all'inizio
    if (*str == '-' || *str == '+') str++;
    
    // Empty string after sign is not a number | Stringa vuota dopo il segno non è un numero
    if (*str == '\0') return 0;
    
    // Check if all remaining characters are digits | Controlla se tutti i caratteri rimanenti sono cifre
    while (*str) {
        if (!isdigit(*str)) return 0;  // Non-digit found | Trovato non-cifra
        str++;
    }
    return 1;  // All checks passed | Tutti i controlli superati
}

/*
 * ============================================================================
 * COMMAND EXECUTOR | ESECUTORE COMANDI
 * ============================================================================
 * 
 * execute_command: Main interpreter function - executes a single token
 * execute_command: Funzione interprete principale - esegue un singolo token
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 *   token - string containing the command or number | stringa contenente il comando o numero
 */
void execute_command(Stack *s, const char *token) {
    /* 
     * NUMBER HANDLING | GESTIONE NUMERI
     * If the token is a number, push it onto the stack
     * Se il token è un numero, inseriscilo nello stack
     */
    if (is_number(token)) {
        stack_push(s, atoi(token));  // Convert string to int and push | Converti stringa in int e inserisci
        return;
    }
    
    /*
     * ========================================================================
     * ARITHMETIC OPERATIONS | OPERAZIONI ARITMETICHE
     * ========================================================================
     * All arithmetic operations pop two values, compute result, and push back
     * Tutte le operazioni aritmetiche estraggono due valori, calcolano il risultato e lo reinseriscono
     */
    
    /* 
     * ADDITION | ADDIZIONE
     * Pop b, pop a, push (a + b)
     */
    if (strcmp(token, "+") == 0) {
        int b = stack_pop(s);  // Second operand | Secondo operando
        int a = stack_pop(s);  // First operand | Primo operando
        stack_push(s, a + b);  // Push result | Inserisci risultato
    }
    
    /* 
     * SUBTRACTION | SOTTRAZIONE
     * Pop b, pop a, push (a - b)
     */
    else if (strcmp(token, "-") == 0) {
        int b = stack_pop(s);  // Value to subtract | Valore da sottrarre
        int a = stack_pop(s);  // Value to subtract from | Valore da cui sottrarre
        stack_push(s, a - b);  // Push difference | Inserisci differenza
    }
    
    /* 
     * MULTIPLICATION | MOLTIPLICAZIONE
     * Pop b, pop a, push (a * b)
     */
    else if (strcmp(token, "*") == 0) {
        int b = stack_pop(s);  // Second factor | Secondo fattore
        int a = stack_pop(s);  // First factor | Primo fattore
        stack_push(s, a * b);  // Push product | Inserisci prodotto
    }
    
    /* 
     * DIVISION | DIVISIONE
     * Pop b, pop a, push (a / b)
     * Note: Integer division with zero check
     * Nota: Divisione intera con controllo per zero
     */
    else if (strcmp(token, "/") == 0) {
        int b = stack_pop(s);  // Divisor | Divisore
        int a = stack_pop(s);  // Dividend | Dividendo
        
        // Prevent division by zero | Previeni divisione per zero
        if (b == 0) {
            fprintf(stderr, "Errore: divisione per zero\n");
            exit(1);
        }
        stack_push(s, a / b);  // Push quotient | Inserisci quoziente
    }
    
    /*
     * ========================================================================
     * STACK MANIPULATION OPERATIONS | OPERAZIONI DI MANIPOLAZIONE STACK
     * ========================================================================
     */
    
    /* 
     * DUP (duplicate): Copy the top element
     * DUP (duplica): Copia l'elemento in cima
     * Stack effect: ( a -- a a )
     */
    else if (strcmp(token, "dup") == 0) {
        int val = stack_peek(s);  // Get top value without removing | Ottieni valore in cima senza rimuoverlo
        stack_push(s, val);       // Push a copy | Inserisci una copia
    }
    
    /* 
     * DROP: Remove the top element
     * DROP: Rimuovi l'elemento in cima
     * Stack effect: ( a -- )
     */
    else if (strcmp(token, "drop") == 0) {
        stack_pop(s);  // Simply remove top element | Semplicemente rimuovi l'elemento in cima
    }
    
    /* 
     * SWAP: Exchange the two top elements
     * SWAP: Scambia i due elementi in cima
     * Stack effect: ( a b -- b a )
     */
    else if (strcmp(token, "swap") == 0) {
        int b = stack_pop(s);  // Remove top | Rimuovi cima
        int a = stack_pop(s);  // Remove second | Rimuovi secondo
        stack_push(s, b);      // Push old top | Inserisci vecchia cima
        stack_push(s, a);      // Push old second | Inserisci vecchio secondo
    }
    
    /* 
     * OVER: Copy the second element to the top
     * OVER: Copia il secondo elemento in cima
     * Stack effect: ( a b -- a b a )
     */
    else if (strcmp(token, "over") == 0) {
        int b = stack_pop(s);    // Remove top temporarily | Rimuovi temporaneamente la cima
        int a = stack_peek(s);   // Peek at second (now top) | Guarda il secondo (ora in cima)
        stack_push(s, b);        // Restore original top | Ripristina la cima originale
        stack_push(s, a);        // Push copy of second | Inserisci copia del secondo
    }
    
    /* 
     * ROT (rotate): Rotate the top three elements
     * ROT (ruota): Ruota i primi tre elementi
     * Stack effect: ( a b c -- b c a )
     * Third element moves to top | Il terzo elemento va in cima
     */
    else if (strcmp(token, "rot") == 0) {
        int c = stack_pop(s);  // Remove top | Rimuovi cima
        int b = stack_pop(s);  // Remove second | Rimuovi secondo
        int a = stack_pop(s);  // Remove third | Rimuovi terzo
        stack_push(s, b);      // b becomes bottom of three | b diventa il fondo dei tre
        stack_push(s, c);      // c becomes middle | c diventa il centro
        stack_push(s, a);      // a becomes top | a diventa la cima
    }
    
    /*
     * ========================================================================
     * I/O OPERATIONS | OPERAZIONI DI INPUT/OUTPUT
     * ========================================================================
     */
    
    /* 
     * PRINT: Pop and display the top element
     * PRINT: Estrai e visualizza l'elemento in cima
     * Stack effect: ( a -- )
     */
    else if (strcmp(token, "print") == 0) {
        printf("%d\n", stack_pop(s));  // Pop and print | Estrai e stampa
    }
    
    /* 
     * .S (dot-s): Display the entire stack without modifying it
     * .S (dot-s): Visualizza l'intero stack senza modificarlo
     * Stack effect: ( ... -- ... )
     */
    else if (strcmp(token, ".s") == 0) {
        print_stack(s);  // Show all elements | Mostra tutti gli elementi
    }
    
    /* 
     * CLEAR: Reset the stack to empty state
     * CLEAR: Resetta lo stack allo stato vuoto
     * Stack effect: ( ... -- )
     */
    else if (strcmp(token, "clear") == 0) {
        stack_init(s);  // Reinitialize stack | Reinizializza lo stack
    }
    
    /*
     * ERROR HANDLING | GESTIONE ERRORI
     * Unknown command | Comando sconosciuto
     */
    else {
        fprintf(stderr, "Errore: comando sconosciuto '%s'\n", token);
        exit(1);  // Terminate on unknown command | Termina su comando sconosciuto
    }
}

/*
 * execute_line: Parse and execute a complete line of code
 * execute_line: Analizza ed esegue una linea completa di codice
 * 
 * Parameters | Parametri:
 *   s - pointer to the stack | puntatore allo stack
 *   line - string containing one or more commands | stringa contenente uno o più comandi
 * 
 * The line is tokenized by whitespace and each token is executed sequentially
 * La linea viene tokenizzata dagli spazi e ogni token viene eseguito in sequenza
 */
void execute_line(Stack *s, const char *line) {
    char buffer[1024];  // Local buffer for tokenization | Buffer locale per la tokenizzazione
    
    // Copy line to buffer safely | Copia la linea nel buffer in modo sicuro
    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination | Assicura terminazione null
    
    // Tokenize by whitespace characters | Tokenizza per caratteri di spaziatura
    char *token = strtok(buffer, " \t\n");
    
    // Execute each token sequentially | Esegui ogni token in sequenza
    while (token != NULL) {
        execute_command(s, token);  // Execute current token | Esegui token corrente
        token = strtok(NULL, " \t\n");  // Get next token | Ottieni prossimo token
    }
}

/*
 * print_help: Display usage information and available commands
 * print_help: Visualizza informazioni d'uso e comandi disponibili
 */
void print_help() {
    printf("Interprete Concatenativo - Comandi disponibili:\n\n");
    printf("Numeri:     <numero>       Push di un numero sullo stack\n");
    printf("Aritmetica: + - * /        Operazioni binarie\n");
    printf("Stack:      dup            Duplica l'elemento in cima\n");
    printf("            drop           Rimuove l'elemento in cima\n");
    printf("            swap           Scambia i primi due elementi\n");
    printf("            over           Copia il secondo elemento in cima\n");
    printf("            rot            Ruota i primi tre elementi\n");
    printf("I/O:        print          Stampa e rimuove l'elemento in cima\n");
    printf("            .s             Mostra lo stack\n");
    printf("            clear          Svuota lo stack\n");
    printf("\nEsempi:\n");
    printf("  5 dup * print          -> 25\n");
    printf("  3 4 + 2 * print        -> 14\n");
    printf("  10 20 swap - print     -> 10\n");
}

/*
 * ============================================================================
 * MAIN FUNCTION | FUNZIONE PRINCIPALE
 * ============================================================================
 * 
 * Entry point of the program - handles both file and interactive modes
 * Punto di ingresso del programma - gestisce sia modalità file che interattiva
 * 
 * Command line arguments | Argomenti da linea di comando:
 *   No arguments: Interactive mode | Nessun argomento: modalità interattiva
 *   <filename>: Execute file | <nomefile>: Esegui file
 *   --help, -h: Display help | --help, -h: Visualizza aiuto
 */
int main(int argc, char *argv[]) {
    Stack stack;      // Declare the main stack | Dichiara lo stack principale
    stack_init(&stack);  // Initialize it to empty | Inizializzalo a vuoto
    
    printf("=== Interprete Concatenativo ===\n");
    
    /*
     * FILE MODE | MODALITÀ FILE
     * If command line argument provided, read and execute from file
     * Se fornito argomento da linea di comando, leggi ed esegui da file
     */
    if (argc > 1) {
        // Check for help flag | Controlla flag di aiuto
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_help();
            return 0;  // Exit successfully | Esci con successo
        }
        
        // Attempt to open the file | Tenta di aprire il file
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Errore: impossibile aprire il file '%s'\n", argv[1]);
            return 1;  // Exit with error | Esci con errore
        }
        
        char line[1024];  // Buffer for reading lines | Buffer per leggere le linee
        printf("Esecuzione da file: %s\n\n", argv[1]);
        
        // Read and execute each line | Leggi ed esegui ogni linea
        while (fgets(line, sizeof(line), file)) {
            // Skip comments (lines starting with #) and empty lines
            // Salta commenti (linee che iniziano con #) e linee vuote
            if (line[0] == '#' || line[0] == '\n') continue;
            
            printf("> %s", line);  // Echo the line | Ripeti la linea
            execute_line(&stack, line);  // Execute it | Eseguila
        }
        fclose(file);  // Close the file | Chiudi il file
        
        // Display final stack state | Visualizza stato finale dello stack
        printf("\nStack finale: ");
        print_stack(&stack);
    }
    
    /*
     * INTERACTIVE MODE | MODALITÀ INTERATTIVA
     * No arguments: enter REPL (Read-Eval-Print Loop)
     * Nessun argomento: entra in REPL (Read-Eval-Print Loop)
     */
    else {
        printf("Modalita' interattiva (scrivi 'help' per i comandi)\n\n");
        
        char line[1024];  // Buffer for user input | Buffer per input utente
        
        // Main REPL loop | Loop REPL principale
        while (1) {
            printf("> ");  // Print prompt | Stampa prompt
            
            // Read a line from stdin | Leggi una linea da stdin
            if (!fgets(line, sizeof(line), stdin)) {
                printf("\n");
                break;  // EOF reached (Ctrl+D) | EOF raggiunto (Ctrl+D)
            }
            
            // Handle special commands | Gestisci comandi speciali
            if (strcmp(line, "help\n") == 0) {
                print_help();
                continue;  // Skip to next iteration | Salta alla prossima iterazione
            }
            
            // Exit commands | Comandi di uscita
            if (strcmp(line, "exit\n") == 0 || strcmp(line, "quit\n") == 0) {
                break;  // Exit REPL | Esci da REPL
            }
            
            // Execute the line | Esegui la linea
            execute_line(&stack, line);
        }
        
        printf("Arrivederci!\n");
    }
    
    return 0;
}