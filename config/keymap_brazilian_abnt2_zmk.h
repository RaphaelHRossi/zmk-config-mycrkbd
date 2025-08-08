/* ========================================
FileName: keymap_brazilian_abnt2_zmk.h
Date: $(date)
Author: ZMK Brazilian Layout
Brief: define keymap for Brazilian ABNT2 layout on ZMK
Use: in your keymap file, include this file
     #include "keymap_brazilian_abnt2_zmk.h"
===========================================

Este arquivo mapeia as teclas do layout ABNT2 brasileiro para funcionar
corretamente com o ZMK, considerando que o OS está configurado para PT-BR.

Layout ABNT2 de referência:
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ ' │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │     │
├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬───┤
│     │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ ´ │ [ │   │
├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐  │
│      │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Ç │ ~ │ ] │  │
├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴──┤
│    │ \ │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ ; │        │
├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬───┤
│    │    │    │                        │    │    │    │   │
└────┴────┴────┴────────────────────────┴────┴────┴────┴───┘
*/

#pragma once

#define XXX &none
#define ___ &trans

// CONTROL CMD/WINDOWS SHIFT ALT/META/OPTION
#define BR_LCTRL &kp LCTRL // left control
#define BR_RCTRL &kp RCTRL // right control
#define BR_LGUI &kp LGUI   // left cmd/windows
#define BR_RGUI &kp RGUI   // right cmd/windows
#define BR_LALT &kp LALT   // left alt
#define BR_RALT &kp RALT   // right alt (AltGr)
#define BR_LSHFT &kp LSHFT // left shift
#define BR_RSHFT &kp RSHFT // right shift

// ARROWS
#define BR_UP &kp UP
#define BR_LEFT &kp LEFT
#define BR_DOWN &kp DOWN
#define BR_RIGHT &kp RIGHT

// SPACE CAPS TAB ENTER BACKSPACE ESCAPE DELETE
#define BR_SPACE &kp SPACE // Space
#define BR_CAPS &kp CAPS   // Caps Lock
#define BR_BSPC &kp BSPC   // Backspace
#define BR_RET &kp RET     // Enter
#define BR_TAB &kp TAB     // Tab
#define BR_ESC &kp ESC     // Escape
#define BR_DEL &kp DEL     // Delete

// PAGE UP PAGE DOWN HOME END
#define BR_PG_UP &kp PG_UP
#define BR_PG_DN &kp PG_DN
#define BR_HOME &kp HOME
#define BR_END &kp END

// FUNCTION KEYS
#define BR_F1 &kp F1
#define BR_F2 &kp F2
#define BR_F3 &kp F3
#define BR_F4 &kp F4
#define BR_F5 &kp F5
#define BR_F6 &kp F6
#define BR_F7 &kp F7
#define BR_F8 &kp F8
#define BR_F9 &kp F9
#define BR_F10 &kp F10
#define BR_F11 &kp F11
#define BR_F12 &kp F12

// NÚMEROS DA LINHA SUPERIOR
#define BR_1 &kp N1       // 1
#define BR_2 &kp N2       // 2
#define BR_3 &kp N3       // 3
#define BR_4 &kp N4       // 4
#define BR_5 &kp N5       // 5
#define BR_6 &kp N6       // 6
#define BR_7 &kp N7       // 7
#define BR_8 &kp N8       // 8
#define BR_9 &kp N9       // 9
#define BR_0 &kp N0       // 0

// CARACTERES ESPECIAIS DA LINHA SUPERIOR
#define BR_QUOT &kp GRAVE    // ' (aspas simples/crase)
#define BR_MINUS &kp MINUS   // - (hífen)
#define BR_EQUAL &kp EQUAL   // = (igual)

// LETRAS
#define BR_Q &kp Q       // Q
#define BR_W &kp W       // W
#define BR_E &kp E       // E
#define BR_R &kp R       // R
#define BR_T &kp T       // T
#define BR_Y &kp Y       // Y
#define BR_U &kp U       // U
#define BR_I &kp I       // I
#define BR_O &kp O       // O
#define BR_P &kp P       // P
#define BR_A &kp A       // A
#define BR_S &kp S       // S
#define BR_D &kp D       // D
#define BR_F &kp F       // F
#define BR_G &kp G       // G
#define BR_H &kp H       // H
#define BR_J &kp J       // J
#define BR_K &kp K       // K
#define BR_L &kp L       // L
#define BR_Z &kp Z       // Z
#define BR_X &kp X       // X
#define BR_C &kp C       // C
#define BR_V &kp V       // V
#define BR_B &kp B       // B
#define BR_N &kp N       // N
#define BR_M &kp M       // M

// CARACTERES ESPECIAIS ABNT2
#define BR_ACUT &kp LBKT     // ´ (acento agudo)
#define BR_LBKT &kp RBKT     // [ (colchete esquerdo)
#define BR_CEDL &kp SEMI     // Ç (cedilha)
#define BR_TILD &kp SQT      // ~ (til)
#define BR_RBKT &kp BSLH     // ] (colchete direito)
#define BR_BSLH &kp NUBS     // \ (barra invertida) - se disponível
#define BR_COMM &kp COMMA    // , (vírgula)
#define BR_DOT &kp DOT       // . (ponto)
#define BR_SCLN &kp FSLH     // ; (ponto e vírgula)
#define BR_FSLH &kp Q        // / (barra) - posição correta no ABNT2

// CARACTERES COM SHIFT
#define BR_DQUO &kp LS(N2)   // " (aspas duplas)
#define BR_AT &kp LS(N3)     // @ (arroba)
#define BR_HASH &kp LS(N4)   // # (jogo da velha)
#define BR_DLR &kp LS(N5)    // $ (dólar)
#define BR_PERC &kp LS(N6)   // % (porcento)
#define BR_CARET &kp LS(N7)  // ¨ (trema)
#define BR_AMPR &kp LS(N8)   // & (e comercial)
#define BR_ASTR &kp LS(N9)   // * (asterisco)
#define BR_LPAR &kp LS(N0)   // ( (parêntese esquerdo)
#define BR_RPAR &kp LS(MINUS) // ) (parêntese direito)
#define BR_UNDS &kp LS(EQUAL) // _ (sublinhado)
#define BR_PLUS &kp LS(EQUAL) // + (mais)
#define BR_GRV &kp LS(LBKT)  // ` (crase)
#define BR_LCBR &kp LS(RBKT) // { (chave esquerda)
#define BR_COLN &kp LS(SEMI) // : (dois pontos)
#define BR_CIRC &kp LS(SQT)  // ^ (circunflexo)
#define BR_RCBR &kp LS(BSLH) // } (chave direita)
#define BR_PIPE &kp LS(NUBS) // | (pipe)
#define BR_LABK &kp LS(COMMA) // < (menor que)
#define BR_RABK &kp LS(DOT)   // > (maior que)
#define BR_QUES &kp LS(MINUS) // ? (interrogação) - posição correta no ABNT2

// CARACTERES COM ALTGR (right alt)
#define BR_SUP2 &kp RA(N2)   // ² (dois ao quadrado)
#define BR_SUP3 &kp RA(N3)   // ³ (três ao quadrado)
#define BR_PND &kp RA(N4)    // £ (libra)
#define BR_CENT &kp RA(N5)   // ¢ (centavo)
#define BR_NOT &kp RA(N6)    // ¬ (não lógico)

// ACENTOS MORTOS (dead keys) - Para uso com combinações
#define BR_DEAD_ACUTE &kp LBKT    // ´ (acento agudo morto)
#define BR_DEAD_GRAVE &kp LS(LBKT) // ` (crase morta)
#define BR_DEAD_TILDE &kp SQT      // ~ (til morto)
#define BR_DEAD_CIRC &kp LS(SQT)   // ^ (circunflexo morto)

// CORREÇÕES PARA LAYOUT ABNT2
// No ABNT2, o ponto e vírgula está na posição onde ficaria a barra no QWERTY US
#define BR_SEMI &kp FSLH        // ; (ponto e vírgula na posição da barra)

// ACENTOS DIRETOS - Para funcionar com o layout ABNT2 no OS
// Essas combinações devem funcionar com dead keys do ABNT2
#define BR_A_ACUTE &macro_tap &kp LBKT &kp A   // ´ + a = á
#define BR_E_ACUTE &macro_tap &kp LBKT &kp E   // ´ + e = é  
#define BR_I_ACUTE &macro_tap &kp LBKT &kp I   // ´ + i = í
#define BR_O_ACUTE &macro_tap &kp LBKT &kp O   // ´ + o = ó
#define BR_U_ACUTE &macro_tap &kp LBKT &kp U   // ´ + u = ú

#define BR_A_GRAVE &macro_tap &kp LS(LBKT) &kp A  // ` + a = à
#define BR_E_GRAVE &macro_tap &kp LS(LBKT) &kp E  // ` + e = è
#define BR_I_GRAVE &macro_tap &kp LS(LBKT) &kp I  // ` + i = ì
#define BR_O_GRAVE &macro_tap &kp LS(LBKT) &kp O  // ` + o = ò
#define BR_U_GRAVE &macro_tap &kp LS(LBKT) &kp U  // ` + u = ù

#define BR_A_TILDE &macro_tap &kp SQT &kp A      // ~ + a = ã
#define BR_O_TILDE &macro_tap &kp SQT &kp O      // ~ + o = õ
#define BR_N_TILDE &macro_tap &kp SQT &kp N      // ~ + n = ñ

#define BR_A_CIRC &macro_tap &kp LS(SQT) &kp A   // ^ + a = â
#define BR_E_CIRC &macro_tap &kp LS(SQT) &kp E   // ^ + e = ê
#define BR_I_CIRC &macro_tap &kp LS(SQT) &kp I   // ^ + i = î
#define BR_O_CIRC &macro_tap &kp LS(SQT) &kp O   // ^ + o = ô
#define BR_U_CIRC &macro_tap &kp LS(SQT) &kp U   // ^ + u = û

// LETRAS ACENTUADAS DIRETAS - Mapeamento correto para ABNT2
// Essas são as posições exatas no layout ABNT2 brasileiro
#define BR_AACUTE &kp LBKT &kp A    // ´ + a = á  
#define BR_EACUTE &kp LBKT &kp E    // ´ + e = é
#define BR_IACUTE &kp LBKT &kp I    // ´ + i = í
#define BR_OACUTE &kp LBKT &kp O    // ´ + o = ó
#define BR_UACUTE &kp LBKT &kp U    // ´ + u = ú

#define BR_AGRAVE &kp LS(LBKT) &kp A // ` + a = à
#define BR_ATILDE &kp SQT &kp A      // ~ + a = ã  
#define BR_OTILDE &kp SQT &kp O      // ~ + o = õ

#define BR_ACIRC &kp LS(SQT) &kp A   // ^ + a = â
#define BR_ECIRC &kp LS(SQT) &kp E   // ^ + e = ê
#define BR_ICIRC &kp LS(SQT) &kp I   // ^ + i = î  
#define BR_OCIRC &kp LS(SQT) &kp O   // ^ + o = ô
#define BR_UCIRC &kp LS(SQT) &kp U   // ^ + u = û

// CARACTERES ESPECIAIS EXTRAS
#define BR_ORDM &kp RA(N0)   // º (ordinal masculino)
#define BR_ORDF &kp RA(N9)   // ª (ordinal feminino)
