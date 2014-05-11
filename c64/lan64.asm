;*********************************************
;  LAN64 Version 2.0
;  By Thorsten Kattanek
;  März 2009

	.save m:\lan64_2_easy
	;.basicloader lan64_2_easy_basicloader.txt

	*=$c000
header  = $8000

intout 	= $bdcd ; Positive Integer Zahl ausgeben (XR/AC)
txtout	= $ab24 ; Textausgeben, anzahl der Zeichen muss im Accu stehen ($22/$23 Adresse des Textes)
ascout	= $ab47 ; Ausgabe eines Asscii Zeichens das im Accu sein muss
cls	= $e544 ; Bildschirm löschen


	sei
	lda #<text00
	sta $22
	lda #>text00
	sta $23
	lda #17
	jsr txtout

	;***** Syncronisation mit dem PC ******

        lda #%00000111          ;Richtung für LAN64 des PortB setzen
	sta $dd03

	lda #00
	sta $dd01		;Portregister B von CIA2 auf Null setzen

wait		
	lda $dd01		;Warten bis alle Eingänge auf 1 sind
	and #%11111000
	cmp #%11111000
	bne wait

	lda #%00000111		;Alle Ausgänge auf 1 stellen
	sta $dd01

wait1		
	lda $dd01		;Warten bis alle Eingänge auf 0 sind
	and #%11111000
	cmp #%00000000
	bne wait1

	;lda #0			;Alle Ausgänge auf 0 stellen
				;Acuu ist in jedem fall 0
	sta $dd01

	;***** Syncronisation ist beendet .... *****

	
	;***** HEADER-BLOCK lesen *******
		
	tay
	lda #<header
	sta $26
	lda #>header
	sta $27
l1
	jsr read_byte
	lda $02
	sta ($26),y
	sta $d020	
	iny 
	bne l1

	lda header+1
	sta $2b
	lda header+2
	sta $2c

	lda header+6
	sta $2d
	sta $ae
	lda header+7
	sta $2e
	sta $af


	;******* Daten Blöcke übertragen *********
	ldy #0
	ldx header+5
	lda header+1
	sta $26
	lda header+2
	sta $27
l2
	jsr read_byte
	lda $02
	sta ($26),y
	sta $d020	
	iny 
	bne l2
	inc $27
	dex
	bne l2

	cli
	rts

;******************* Unterprogramme ***********************

;**** Byte holen ****
read_byte

wait_lo_byte

        lda $dd01
        and #%00001000
        beq wait_lo_byte

	lda $dd01
	lsr
	lsr
	lsr
	lsr
	sta $02

	lda #%00000100
	sta $dd01

wait_hi_byte

	lda $dd01
	and #%00001000
	bne wait_hi_byte

	lda $dd01
	and #%11110000
	ora $02
	sta $02	

	lda #%00000000
	sta $dd01
	
	;** In $02 befindet sich das komplette Byte
	rts

text00
	.byte 5
	.text "WARTEN AUF EMU64"

copy
	ldx #0
copy_1
	lda $c000,x
	sta 288,x
	inx
	cpx #$C0
	bne copy_1
	lda #1
	sta 294
	sta 349
	sta 401
	lda #163
	sta 348
	sta 400
	lda #206
	sta 290
	rts
save
	ldx #$08
	ldy #$01
	jsr $ffba
	lda #$05
	ldx #<text01
	ldy #>text01
	jsr $ffbd
	ldx #$20
	ldy #$01
	stx $fb
	sty $fc
	lda #$fb
	ldx #$df
	ldy #$01
	jsr $ffd8
	rts
text01
	.text "LAN64"
	

.var copy
.var save
