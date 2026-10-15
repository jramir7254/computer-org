#ifndef PITCHES_H
#define PITCHES_H

/* Note frequencies in Hz, rounded to the nearest whole number.
 * Covers all 88 keys of a grand piano: A0 (key 1) to C8 (key 88).
 *
 * To play one on the buzzer (2 MHz timer clock), convert Hz to a period:
 *     buzzer_set_period(2000000L / NOTE_A4);
 *
 * The timer can only count to 65535, so the lowest note it can play is
 * about 31 Hz (B0). A0 and AS0 are listed for completeness but are too low
 * for this buzzer setup.
 */

#define NOTE_REST 0      /* silence in a melody */

/* Octave 0 */
#define NOTE_A0   28    /* key  1:    27.50 Hz */
#define NOTE_AS0  29    /* key  2:    29.14 Hz */
#define NOTE_B0   31    /* key  3:    30.87 Hz */

/* Octave 1 */
#define NOTE_C1   33    /* key  4:    32.70 Hz */
#define NOTE_CS1  35    /* key  5:    34.65 Hz */
#define NOTE_D1   37    /* key  6:    36.71 Hz */
#define NOTE_DS1  39    /* key  7:    38.89 Hz */
#define NOTE_E1   41    /* key  8:    41.20 Hz */
#define NOTE_F1   44    /* key  9:    43.65 Hz */
#define NOTE_FS1  46    /* key 10:    46.25 Hz */
#define NOTE_G1   49    /* key 11:    49.00 Hz */
#define NOTE_GS1  52    /* key 12:    51.91 Hz */
#define NOTE_A1   55    /* key 13:    55.00 Hz */
#define NOTE_AS1  58    /* key 14:    58.27 Hz */
#define NOTE_B1   62    /* key 15:    61.74 Hz */

/* Octave 2 */
#define NOTE_C2   65    /* key 16:    65.41 Hz */
#define NOTE_CS2  69    /* key 17:    69.30 Hz */
#define NOTE_D2   73    /* key 18:    73.42 Hz */
#define NOTE_DS2  78    /* key 19:    77.78 Hz */
#define NOTE_E2   82    /* key 20:    82.41 Hz */
#define NOTE_F2   87    /* key 21:    87.31 Hz */
#define NOTE_FS2  92    /* key 22:    92.50 Hz */
#define NOTE_G2   98    /* key 23:    98.00 Hz */
#define NOTE_GS2  104   /* key 24:   103.83 Hz */
#define NOTE_A2   110   /* key 25:   110.00 Hz */
#define NOTE_AS2  117   /* key 26:   116.54 Hz */
#define NOTE_B2   123   /* key 27:   123.47 Hz */

/* Octave 3 */
#define NOTE_C3   131   /* key 28:   130.81 Hz */
#define NOTE_CS3  139   /* key 29:   138.59 Hz */
#define NOTE_D3   147   /* key 30:   146.83 Hz */
#define NOTE_DS3  156   /* key 31:   155.56 Hz */
#define NOTE_E3   165   /* key 32:   164.81 Hz */
#define NOTE_F3   175   /* key 33:   174.61 Hz */
#define NOTE_FS3  185   /* key 34:   185.00 Hz */
#define NOTE_G3   196   /* key 35:   196.00 Hz */
#define NOTE_GS3  208   /* key 36:   207.65 Hz */
#define NOTE_A3   220   /* key 37:   220.00 Hz */
#define NOTE_AS3  233   /* key 38:   233.08 Hz */
#define NOTE_B3   247   /* key 39:   246.94 Hz */

/* Octave 4 */
#define NOTE_C4   262   /* key 40:   261.63 Hz */
#define NOTE_CS4  277   /* key 41:   277.18 Hz */
#define NOTE_D4   294   /* key 42:   293.66 Hz */
#define NOTE_DS4  311   /* key 43:   311.13 Hz */
#define NOTE_E4   330   /* key 44:   329.63 Hz */
#define NOTE_F4   349   /* key 45:   349.23 Hz */
#define NOTE_FS4  370   /* key 46:   369.99 Hz */
#define NOTE_G4   392   /* key 47:   392.00 Hz */
#define NOTE_GS4  415   /* key 48:   415.30 Hz */
#define NOTE_A4   440   /* key 49:   440.00 Hz */
#define NOTE_AS4  466   /* key 50:   466.16 Hz */
#define NOTE_B4   494   /* key 51:   493.88 Hz */

/* Octave 5 */
#define NOTE_C5   523   /* key 52:   523.25 Hz */
#define NOTE_CS5  554   /* key 53:   554.37 Hz */
#define NOTE_D5   587   /* key 54:   587.33 Hz */
#define NOTE_DS5  622   /* key 55:   622.25 Hz */
#define NOTE_E5   659   /* key 56:   659.26 Hz */
#define NOTE_F5   698   /* key 57:   698.46 Hz */
#define NOTE_FS5  740   /* key 58:   739.99 Hz */
#define NOTE_G5   784   /* key 59:   783.99 Hz */
#define NOTE_GS5  831   /* key 60:   830.61 Hz */
#define NOTE_A5   880   /* key 61:   880.00 Hz */
#define NOTE_AS5  932   /* key 62:   932.33 Hz */
#define NOTE_B5   988   /* key 63:   987.77 Hz */

/* Octave 6 */
#define NOTE_C6   1047  /* key 64:  1046.50 Hz */
#define NOTE_CS6  1109  /* key 65:  1108.73 Hz */
#define NOTE_D6   1175  /* key 66:  1174.66 Hz */
#define NOTE_DS6  1245  /* key 67:  1244.51 Hz */
#define NOTE_E6   1319  /* key 68:  1318.51 Hz */
#define NOTE_F6   1397  /* key 69:  1396.91 Hz */
#define NOTE_FS6  1480  /* key 70:  1479.98 Hz */
#define NOTE_G6   1568  /* key 71:  1567.98 Hz */
#define NOTE_GS6  1661  /* key 72:  1661.22 Hz */
#define NOTE_A6   1760  /* key 73:  1760.00 Hz */
#define NOTE_AS6  1865  /* key 74:  1864.66 Hz */
#define NOTE_B6   1976  /* key 75:  1975.53 Hz */

/* Octave 7 */
#define NOTE_C7   2093  /* key 76:  2093.00 Hz */
#define NOTE_CS7  2217  /* key 77:  2217.46 Hz */
#define NOTE_D7   2349  /* key 78:  2349.32 Hz */
#define NOTE_DS7  2489  /* key 79:  2489.02 Hz */
#define NOTE_E7   2637  /* key 80:  2637.02 Hz */
#define NOTE_F7   2794  /* key 81:  2793.83 Hz */
#define NOTE_FS7  2960  /* key 82:  2959.96 Hz */
#define NOTE_G7   3136  /* key 83:  3135.96 Hz */
#define NOTE_GS7  3322  /* key 84:  3322.44 Hz */
#define NOTE_A7   3520  /* key 85:  3520.00 Hz */
#define NOTE_AS7  3729  /* key 86:  3729.31 Hz */
#define NOTE_B7   3951  /* key 87:  3951.07 Hz */

/* Octave 8 */
#define NOTE_C8   4186  /* key 88:  4186.01 Hz */

/* Above the piano's top key (kept so older code that uses them still builds) */
#define NOTE_CS8  4435  /* extra :  4434.92 Hz */
#define NOTE_D8   4699  /* extra :  4698.64 Hz */
#define NOTE_DS8  4978  /* extra :  4978.03 Hz */

#endif
