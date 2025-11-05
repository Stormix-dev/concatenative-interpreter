# 🔗 Interprete per Linguaggio Concatenativo

Un interprete stack-based per linguaggio concatenativo scritto in C, disponibile in bilingue (italiano/inglese).

## 🚀 Quick Start

### Compilazione

```bash
gcc -o concat_exec concat_interpreter.c -Wall

# Oppure usa il Makefile
make
```

### Esecuzione

```bash
# Modalità interattiva
./concat_exec

# Esecuzione da file
./concat_exec examples.cat

# Aiuto
./concat_exec --help

# Versione
./concat_exec --version
```

## 📚 Guida ai Comandi

### Versione Base

| Comando | Descrizione | Stack Effect | Esempio |
|---------|-------------|--------------|---------|
| `<n>` | Push numero | `( -- n )` | `42` |
| `+` | Addizione | `( a b -- a+b )` | `3 4 +` → 7 |
| `-` | Sottrazione | `( a b -- a-b )` | `10 3 -` → 7 |
| `*` | Moltiplicazione | `( a b -- a*b )` | `5 6 *` → 30 |
| `/` | Divisione | `( a b -- a/b )` | `20 4 /` → 5 |
| `dup` | Duplica | `( a -- a a )` | `5 dup` → [5, 5] |
| `drop` | Rimuovi | `( a -- )` | `1 2 drop` → [1] |
| `swap` | Scambia | `( a b -- b a )` | `1 2 swap` → [2, 1] |
| `over` | Copia secondo | `( a b -- a b a )` | `1 2 over` → [1, 2, 1] |
| `rot` | Ruota tre | `( a b c -- b c a )` | `1 2 3 rot` → [2, 3, 1] |
| `print` | Stampa top | `( a -- )` | `42 print` |
| `.s` | Mostra stack | `( ... -- ... )` | `.s` |
| `clear` | Svuota stack | `( ... -- )` | `clear` |

### Versione Estesa (Comandi Aggiuntivi)

| Comando | Descrizione | Stack Effect | Esempio |
|---------|-------------|--------------|---------|
| `mod` | Modulo | `( a b -- a%b )` | `17 5 mod` → 2 |
| `neg` | Negazione | `( a -- -a )` | `5 neg` → -5 |
| `abs` | Valore assoluto | `( a -- \|a\| )` | `-10 abs` → 10 |
| `eq` | Uguaglianza | `( a b -- a==b )` | `5 5 eq` → 1 |
| `lt` | Minore | `( a b -- a<b )` | `3 5 lt` → 1 |
| `gt` | Maggiore | `( a b -- a>b )` | `7 3 gt` → 1 |
| `nip` | Rimuovi secondo | `( a b -- b )` | `1 2 nip` → [2] |
| `tuck` | Copia sotto | `( a b -- b a b )` | `1 2 tuck` → [2, 1, 2] |
| `2dup` | Duplica due | `( a b -- a b a b )` | `1 2 2dup` → [1, 2, 1, 2] |
| `min` | Minimo | `( a b -- min(a,b) )` | `10 5 min` → 5 |
| `max` | Massimo | `( a b -- max(a,b) )` | `10 5 max` → 10 |
| `random` | Casuale | `( n -- rand(0,n) )` | `100 random` |
| `depth` | Profondità | `( ... -- ... n )` | `1 2 3 depth` → 3 |
| `stats` | Statistiche | `( ... -- ... )` | `stats` |

## 💡 Esempi Pratici

### Quadrato di un numero
```
5 dup * print
→ 25
```

### Media di due numeri
```
10 20 + 2 / print
→ 15
```

### Controllare se un numero è pari
```
17 2 mod 0 eq print
→ 0 (falso, 17 è dispari)
```

### Valore assoluto di una differenza
```
15 22 - abs print
→ 7
```

### Espressione complessa: ((10 + 5) * 3 - 20) / 5
```
10 5 + 3 * 20 - 5 / print
→ 5
```

### Trovare il massimo tra tre numeri
```
10 20 max 15 max print
→ 20
```

## 🔍 Come Funziona

### Il concetto di linguaggio concatenativo

I linguaggi concatenativi si basano sulla **composizione di funzioni** e utilizzano uno **stack** come unica struttura dati. Ogni operazione:

1. **Preleva** uno o più elementi dallo stack
2. **Elabora** i dati
3. **Reinserisce** il risultato sullo stack

Esempio di valutazione di `5 dup * print`:
```
Passo 1: 5        → Stack: [5]
Passo 2: dup      → Stack: [5, 5]
Passo 3: *        → Stack: [25]
Passo 4: print    → Output: 25, Stack: []
```

### Architettura dell'Interprete

1. **Stack**: Array di interi con puntatore al top
2. **Tokenizer**: Separa l'input in token (parole)
3. **Executor**: Esegue ogni token sequenzialmente
4. **Error Handler**: Gestisce overflow, underflow, divisione per zero

## 🎯 Caratteristiche Uniche

Questa implementazione include diversi elementi che la rendono unica:

- ✨ **Commenti bilingue** su ogni singola riga di codice
- ✨ **Contatore operazioni** per tracking delle performance
- ✨ **Comandi statistiche** per analizzare l'utilizzo
- ✨ **Operazioni estese** non comuni in altri interpreti base
- ✨ **Gestione errori robusta** con messaggi chiari
- ✨ **Supporto commenti** nei file sorgente (linee con #)

## 🛠️ Miglioramenti Futuri

- [ ] Definizione di parole custom (`:` e `;`)
- [ ] Strutture di controllo (if-then-else, loop)
- [ ] Variabili e memoria persistente
- [ ] Supporto floating-point
- [ ] Array e stringhe
- [ ] Garbage collection per strutture complesse
- [ ] Debugger integrato
- [ ] REPL history con frecce su/giù

## 📊 Complessità

- **Push/Pop**: O(1)
- **Operazioni aritmetiche**: O(1)
- **Tokenizzazione**: O(n) dove n = lunghezza input
- **Spazio**: O(1) - stack fisso di 256 elementi

## 🧪 Testing

```bash
# Test 
./concat_exec examples.cat

# Test interattivo manuale
./concat_exec
> 5 dup * print
25
> 10 20 30 .s
Stack[3]: [ 10 20 30 ]
> stats
```

## 📝 Note Tecniche

- **Linguaggio**: C (C99 standard)
- **Dipendenze**: Solo librerie standard
- **Portabilità**: Linux, macOS, Windows (con MinGW)
- **Stack size**: 256 elementi (configurabile via MAX_STACK)
- **Tipo dati**: Solo interi (int)

## 🤝 Contribuire

Questo è un progetto didattico per portfolio. Sentiti libero di:
- 🔧 Fare fork e modificare
- 💡 Suggerire miglioramenti
- 🐛 Segnalare bug
- 📚 Migliorare la documentazione

## 📄 Licenza

MIT License - Usa liberamente per scopi educativi e portfolio.

## 👤 Autore

**[Il tuo nome]**
- GitHub: [@stormix-dev]
- Portfolio: [https://stormix-dev.github.io/]

---

*Interprete Concatenativo - Un progetto educativo per comprendere i fondamenti dei linguaggi di programmazione*
