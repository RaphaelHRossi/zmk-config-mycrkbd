/* ========================================
FileName: keymap_brazilian_abnt2_zmk.h
Brief: keycodes para o layout Português (Brasil ABNT2) no ZMK
Use: #include "keymap_brazilian_abnt2_zmk.h" no seu .keymap
===========================================

COMO FUNCIONA
O teclado NÃO envia caracteres, envia a POSIÇÃO da tecla (HID usage).
Quem transforma posição em caractere é o sistema operacional, de acordo com
o layout configurado. Este arquivo assume que o SO está em
"Português (Brasil ABNT2)" e diz qual POSIÇÃO enviar para cada caractere.

As duas teclas que não existem num teclado americano:
  \ |  -> NUBS  (HID 0x64, a tecla ao lado do Shift esquerdo no ABNT2)
  / ?  -> INT1  (HID 0x87, a tecla ao lado do Shift direito no ABNT2)

NUNCA use &kp SLASH / FSLH / QMARK para / e ?: no ABNT2 essa posição é ; e :
NUNCA use códigos Alt+numpad (Alt+047, Alt+063): dependem do NumLock e do
aplicativo, e quando falham o Alt vira atalho (abre menu, dispara comandos).

Obs.: INT1 exige o relatório HKRO (padrão do ZMK). Se um dia ativar
CONFIG_ZMK_HID_REPORT_TYPE_NKRO, ative também
CONFIG_ZMK_HID_KEYBOARD_NKRO_EXTENDED_REPORT=y, senão / e ? param de funcionar.

Layout ABNT2 de referência:
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ ' │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │     │
├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬───┤
│     │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ ´ │ [ │   │
├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐  │
│      │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Ç │ ~ │ ] │  │
├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴──┤
│    │ \ │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ ; │ / │    │
└────┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴────┘
*/

#pragma once

#define XXX &none
#define ___ &trans

// MODIFICADORES
#define BR_LCTRL &kp LCTRL
#define BR_RCTRL &kp RCTRL
#define BR_LGUI  &kp LGUI
#define BR_RGUI  &kp RGUI
#define BR_LALT  &kp LALT
#define BR_RALT  &kp RALT   // AltGr
#define BR_LSHFT &kp LSHFT
#define BR_RSHFT &kp RSHFT

// SETAS
#define BR_UP    &kp UP
#define BR_LEFT  &kp LEFT
#define BR_DOWN  &kp DOWN
#define BR_RIGHT &kp RIGHT

// SPACE CAPS TAB ENTER BACKSPACE ESCAPE DELETE
#define BR_SPACE &kp SPACE
#define BR_CAPS  &kp CAPS
#define BR_BSPC  &kp BSPC
#define BR_RET   &kp RET
#define BR_TAB   &kp TAB
#define BR_ESC   &kp ESC
#define BR_DEL   &kp DEL

// PAGE UP PAGE DOWN HOME END
#define BR_PG_UP &kp PG_UP
#define BR_PG_DN &kp PG_DN
#define BR_HOME  &kp HOME
#define BR_END   &kp END

// TECLAS DE FUNÇÃO
#define BR_F1  &kp F1
#define BR_F2  &kp F2
#define BR_F3  &kp F3
#define BR_F4  &kp F4
#define BR_F5  &kp F5
#define BR_F6  &kp F6
#define BR_F7  &kp F7
#define BR_F8  &kp F8
#define BR_F9  &kp F9
#define BR_F10 &kp F10
#define BR_F11 &kp F11
#define BR_F12 &kp F12

// NÚMEROS
#define BR_1 &kp N1
#define BR_2 &kp N2
#define BR_3 &kp N3
#define BR_4 &kp N4
#define BR_5 &kp N5
#define BR_6 &kp N6
#define BR_7 &kp N7
#define BR_8 &kp N8
#define BR_9 &kp N9
#define BR_0 &kp N0

// LETRAS
#define BR_Q &kp Q
#define BR_W &kp W
#define BR_E &kp E
#define BR_R &kp R
#define BR_T &kp T
#define BR_Y &kp Y
#define BR_U &kp U
#define BR_I &kp I
#define BR_O &kp O
#define BR_P &kp P
#define BR_A &kp A
#define BR_S &kp S
#define BR_D &kp D
#define BR_F &kp F
#define BR_G &kp G
#define BR_H &kp H
#define BR_J &kp J
#define BR_K &kp K
#define BR_L &kp L
#define BR_Z &kp Z
#define BR_X &kp X
#define BR_C &kp C
#define BR_V &kp V
#define BR_B &kp B
#define BR_N &kp N
#define BR_M &kp M

// SÍMBOLOS SEM SHIFT (posição física no ABNT2 -> código HID)
#define BR_QUOT  &kp GRAVE   // '
#define BR_MINUS &kp MINUS   // -
#define BR_EQUAL &kp EQUAL   // =
#define BR_ACUT  &kp LBKT    // ´ (tecla morta)
#define BR_LBKT  &kp RBKT    // [
#define BR_CEDL  &kp SEMI    // Ç
#define BR_TILD  &kp SQT     // ~ (tecla morta)
#define BR_RBKT  &kp BSLH    // ]
#define BR_BSLH  &kp NUBS    // \  (tecla ao lado do Shift esquerdo)
#define BR_COMM  &kp COMMA   // ,
#define BR_DOT   &kp DOT     // .
#define BR_SCLN  &kp FSLH    // ;  (no ABNT2 a posição "/" americana é o ;)
#define BR_SEMI  BR_SCLN
#define BR_FSLH  &kp INT1    // /  (tecla ao lado do Shift direito)  <-- CORREÇÃO

// SÍMBOLOS COM SHIFT
#define BR_DQUO  &kp LS(GRAVE)  // "
#define BR_EXCL  &kp LS(N1)     // !
#define BR_AT    &kp LS(N2)     // @
#define BR_HASH  &kp LS(N3)     // #
#define BR_DLR   &kp LS(N4)     // $
#define BR_PERC  &kp LS(N5)     // %
#define BR_DIAE  &kp LS(N6)     // ¨ (tecla morta)
#define BR_CARET BR_DIAE        // nome antigo mantido por compatibilidade
#define BR_AMPR  &kp LS(N7)     // &
#define BR_ASTR  &kp LS(N8)     // *
#define BR_LPAR  &kp LS(N9)     // (
#define BR_RPAR  &kp LS(N0)     // )
#define BR_UNDS  &kp LS(MINUS)  // _
#define BR_PLUS  &kp LS(EQUAL)  // +
#define BR_GRV   &kp LS(LBKT)   // ` (tecla morta)
#define BR_LCBR  &kp LS(RBKT)   // {
#define BR_CIRC  &kp LS(SQT)    // ^ (tecla morta)
#define BR_RCBR  &kp LS(BSLH)   // }
#define BR_PIPE  &kp LS(NUBS)   // |
#define BR_LABK  &kp LS(COMMA)  // <
#define BR_RABK  &kp LS(DOT)    // >
#define BR_COLN  &kp LS(FSLH)   // :
#define BR_QUES  &kp LS(INT1)   // ?  <-- CORREÇÃO

// SÍMBOLOS COM ALTGR
#define BR_SUP1  &kp RA(N1)     // ¹
#define BR_SUP2  &kp RA(N2)     // ²
#define BR_SUP3  &kp RA(N3)     // ³
#define BR_PND   &kp RA(N4)     // £
#define BR_CENT  &kp RA(N5)     // ¢
#define BR_NOT   &kp RA(N6)     // ¬
#define BR_SECT  &kp RA(EQUAL)  // §
#define BR_ORDF  &kp RA(RBKT)   // ª
#define BR_ORDM  &kp RA(BSLH)   // º
#define BR_DEG   &kp RA(E)      // °

// TECLAS MORTAS (aliases)
#define BR_DEAD_ACUTE BR_ACUT
#define BR_DEAD_GRAVE BR_GRV
#define BR_DEAD_TILDE BR_TILD
#define BR_DEAD_CIRC  BR_CIRC
