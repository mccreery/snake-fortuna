.text
.global hue192to565
.type hue192to565, @function

; divides the circle into 6 segments of 32 values each.
; the segments are labelled 0-5, ie. segment 0 spans [0, 60) degrees
#define hue r24
#define red r20
#define green r21
#define blue r22
#define tmp r23
#define lo r24
#define hi r25

; swap two registers using a tmp
.macro swapreg a, b
    mov tmp, \a
    mov \a, \b
    mov \b, tmp
.endm

hue192to565:
    ; load starting constants
    ldi red, 31
    mov green, hue
    clr blue

    ; prepare slope
    ; on odd numbered segments (60deg, 180deg...)
    sbrc hue, 5
    com green
    ; dual purpose cycle: clears extra bits from hue and from complement
    andi green, 0x1f

    ; invert flats on top 3 segments
    cpi hue, 96
    brlo swap1
    subi hue, 96
    clr red
    ldi blue, 31
swap1:
    ; rotate by swapping (first rotation: segments 1 and 4)
    cpi hue, 32
    brlo swap2
    swapreg red, green
swap2:
    ; second rotation: segments 2 and 5
    cpi hue, 64
    brlo pack
    swapreg red, blue
pack:
    ; L = b | g << 6 | g:4 << 5
    mov lo, blue

    ; tmp = g << 6
    mov tmp, green
    swap tmp
    lsl tmp
    lsl tmp
    andi tmp, 0xfe

    or lo, tmp
    ; increment top half
    sbrc green, 4
    sbr lo, 0x20

    ; H = g >> 2 | r << 3
    mov hi, green
    lsr hi
    lsr hi
    lsl red
    lsl red
    lsl red
    or hi, red

    ret
.size hue192to565, .-hue192to565
