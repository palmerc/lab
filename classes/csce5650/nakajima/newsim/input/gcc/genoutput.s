gcc2_compiled.:
___gnu_compiled_c:
	.global _rtl_obstack
.data
	.align 4
_rtl_obstack:
	.word	_obstack
.text
	.align 8
LC0:
	.ascii "/* Generated automatically by the program `genoutput'\12from the machine description file `md'.  */\12\12\0"
	.align 8
LC1:
	.ascii "#include \"config.h\"\12\0"
	.align 8
LC2:
	.ascii "#include \"rtl.h\"\12\0"
	.align 8
LC3:
	.ascii "#include \"regs.h\"\12\0"
	.align 8
LC4:
	.ascii "#include \"hard-reg-set.h\"\12\0"
	.align 8
LC5:
	.ascii "#include \"real.h\"\12\0"
	.align 8
LC6:
	.ascii "#include \"conditions.h\"\12\0"
	.align 8
LC7:
	.ascii "#include \"insn-flags.h\"\12\0"
	.align 8
LC8:
	.ascii "#include \"insn-config.h\"\12\12\0"
	.align 8
LC9:
	.ascii "#ifndef __STDC__\12\0"
	.align 8
LC10:
	.ascii "#define const\12\0"
	.align 8
LC11:
	.ascii "#endif\12\12\0"
	.align 8
LC12:
	.ascii "#include \"output.h\"\12\0"
	.align 8
LC13:
	.ascii "#include \"aux-output.c\"\12\12\0"
	.align 8
LC14:
	.ascii "#ifndef INSN_MACHINE_INFO\12\0"
	.align 8
LC15:
	.ascii "#define INSN_MACHINE_INFO struct dummy1 {int i;}\12\0"
	.align 4
	.global _output_prologue
	.proc	020
_output_prologue:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	sethi %hi(LC0),%o0
	call _printf,0
	or %o0,%lo(LC0),%o0
	sethi %hi(LC1),%o0
	call _printf,0
	or %o0,%lo(LC1),%o0
	sethi %hi(LC2),%o0
	call _printf,0
	or %o0,%lo(LC2),%o0
	sethi %hi(LC3),%o0
	call _printf,0
	or %o0,%lo(LC3),%o0
	sethi %hi(LC4),%o0
	call _printf,0
	or %o0,%lo(LC4),%o0
	sethi %hi(LC5),%o0
	call _printf,0
	or %o0,%lo(LC5),%o0
	sethi %hi(LC6),%o0
	call _printf,0
	or %o0,%lo(LC6),%o0
	sethi %hi(LC7),%o0
	call _printf,0
	or %o0,%lo(LC7),%o0
	sethi %hi(LC8),%o0
	call _printf,0
	or %o0,%lo(LC8),%o0
	sethi %hi(LC9),%o0
	call _printf,0
	or %o0,%lo(LC9),%o0
	sethi %hi(LC10),%o0
	call _printf,0
	or %o0,%lo(LC10),%o0
	sethi %hi(LC11),%l0
	call _printf,0
	or %l0,%lo(LC11),%o0
	sethi %hi(LC12),%o0
	call _printf,0
	or %o0,%lo(LC12),%o0
	sethi %hi(LC13),%o0
	call _printf,0
	or %o0,%lo(LC13),%o0
	sethi %hi(LC14),%o0
	call _printf,0
	or %o0,%lo(LC14),%o0
	sethi %hi(LC15),%o0
	call _printf,0
	or %o0,%lo(LC15),%o0
	call _printf,0
	or %l0,%lo(LC11),%o0
	ret
	restore
	.align 8
LC16:
	.ascii "\12char * const insn_template[] =\12  {\12\0"
	.align 8
LC17:
	.ascii "    \"%s\",\12\0"
	.align 8
LC18:
	.ascii "    0,\12\0"
	.align 8
LC19:
	.ascii "  };\12\0"
	.align 8
LC20:
	.ascii "\12char *(*const insn_outfun[])() =\12  {\12\0"
	.align 8
LC21:
	.ascii "    output_%d,\12\0"
	.align 8
LC22:
	.ascii "\12rtx (*const insn_gen_function[]) () =\12  {\12\0"
	.align 8
LC23:
	.ascii "    gen_%s,\12\0"
	.align 8
LC24:
	.ascii "\12const int insn_n_operands[] =\12  {\12\0"
	.align 8
LC25:
	.ascii "    %d,\12\0"
	.align 8
LC26:
	.ascii "\12const int insn_n_dups[] =\12  {\12\0"
	.align 8
LC27:
	.ascii "\12char *const insn_operand_constraint[][MAX_RECOG_OPERANDS] =\12  {\12\0"
	.align 8
LC28:
	.ascii "    {\0"
	.align 8
LC29:
	.ascii "wrong number of alternatives in operand %d of insn number %d\0"
	.align 8
LC30:
	.ascii " \"\",\0"
	.align 8
LC31:
	.ascii " \"%s\",\0"
	.align 8
LC32:
	.ascii " 0\0"
	.align 8
LC33:
	.ascii " },\12\0"
	.align 8
LC34:
	.ascii "\12const char insn_operand_address_p[][MAX_RECOG_OPERANDS] =\12  {\12\0"
	.align 8
LC35:
	.ascii " %d,\0"
	.align 8
LC36:
	.ascii "\12const enum machine_mode insn_operand_mode[][MAX_RECOG_OPERANDS] =\12  {\12\0"
	.align 8
LC37:
	.ascii " %smode,\0"
	.align 8
LC38:
	.ascii " VOIDmode\0"
	.align 8
LC39:
	.ascii "\12const char insn_operand_strict_low[][MAX_RECOG_OPERANDS] =\12  {\12\0"
	.align 8
LC40:
	.ascii "\12int (*const insn_operand_predicate[][MAX_RECOG_OPERANDS])() =\12  {\12\0"
	.align 8
LC41:
	.ascii " %s,\0"
	.align 8
LC42:
	.ascii "0\0"
	.align 8
LC43:
	.ascii "\12const INSN_MACHINE_INFO insn_machine_info[] =\12  {\12\0"
	.align 8
LC44:
	.ascii "    {%s},\12\0"
	.align 8
LC45:
	.ascii "     {0},\12\0"
	.align 8
LC46:
	.ascii "\12const int insn_n_alternatives[] =\12  {\12\0"
	.align 8
LC47:
	.ascii "     0,\12\0"
	.align 4
	.global _output_epilogue
	.proc	020
_output_epilogue:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	sethi %hi(LC16),%o0
	call _printf,0
	or %o0,%lo(LC16),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L117
	sethi %hi(LC19),%o0
	sethi %hi(LC17),%l2
	sethi %hi(LC18),%l0
	ld [%l1+8],%o1
L119:
	cmp %o1,0
	be L6
	nop
	call _printf,0
	or %l2,%lo(LC17),%o0
	b L118
	ld [%l1+24],%l1
L6:
	call _printf,0
	or %l0,%lo(LC18),%o0
	ld [%l1+24],%l1
L118:
	cmp %l1,0
	bne,a L119
	ld [%l1+8],%o1
	sethi %hi(LC19),%o0
L117:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC20),%o0
	call _printf,0
	or %o0,%lo(LC20),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L120
	sethi %hi(LC19),%o0
	sethi %hi(LC21),%l2
	sethi %hi(LC18),%l0
	ldsb [%l1+748],%o0
L122:
	cmp %o0,0
	be L12
	nop
	ld [%l1],%o1
	call _printf,0
	or %l2,%lo(LC21),%o0
	b L121
	ld [%l1+24],%l1
L12:
	call _printf,0
	or %l0,%lo(LC18),%o0
	ld [%l1+24],%l1
L121:
	cmp %l1,0
	bne,a L122
	ldsb [%l1+748],%o0
	sethi %hi(LC19),%o0
L120:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC22),%o0
	call _printf,0
	or %o0,%lo(LC22),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L123
	sethi %hi(LC19),%o0
	sethi %hi(LC23),%l2
	sethi %hi(LC18),%l0
	ld [%l1+4],%o1
L125:
	cmp %o1,0
	be L18
	nop
	call _printf,0
	or %l2,%lo(LC23),%o0
	b L124
	ld [%l1+24],%l1
L18:
	call _printf,0
	or %l0,%lo(LC18),%o0
	ld [%l1+24],%l1
L124:
	cmp %l1,0
	bne,a L125
	ld [%l1+4],%o1
	sethi %hi(LC19),%o0
L123:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC24),%o0
	call _printf,0
	or %o0,%lo(LC24),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L126
	sethi %hi(LC19),%o0
	sethi %hi(LC25),%l0
	ld [%l1+12],%o1
L127:
	call _printf,0
	or %l0,%lo(LC25),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne,a L127
	ld [%l1+12],%o1
	sethi %hi(LC19),%o0
L126:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC26),%o0
	call _printf,0
	or %o0,%lo(LC26),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L128
	sethi %hi(LC19),%o0
	sethi %hi(LC25),%l0
	ld [%l1+16],%o1
L129:
	call _printf,0
	or %l0,%lo(LC25),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne,a L129
	ld [%l1+16],%o1
	sethi %hi(LC19),%o0
L128:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(_have_constraints),%o0
	ld [%o0+%lo(_have_constraints)],%o0
	cmp %o0,0
	be,a L29
	sethi %hi(LC34),%o0
	sethi %hi(LC27),%o0
	call _printf,0
	or %o0,%lo(LC27),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L51
	sethi %hi(LC29),%l6
	sethi %hi(LC30),%l5
	sethi %hi(LC31),%l4
	mov 0,%l3
L134:
	sethi %hi(LC28),%o0
	call _printf,0
	or %o0,%lo(LC28),%o0
	ld [%l1+12],%o0
	cmp %l3,%o0
	bge L34
	mov 0,%l0
	mov %l1,%l2
L40:
	ld [%l2+188],%o0
	cmp %o0,0
	ble,a L130
	ld [%l1+12],%o0
	cmp %l3,0
	bne L37
	cmp %l3,%o0
	b L35
	mov %o0,%l3
L37:
	be,a L130
	ld [%l1+12],%o0
	or %l6,%lo(LC29),%o0
	ld [%l1],%o2
	call _error,0
	mov %l0,%o1
L35:
	ld [%l1+12],%o0
L130:
	add %l0,1,%l0
	cmp %l0,%o0
	bl L40
	add %l2,4,%l2
L34:
	st %l3,[%l1+20]
	ld [%l1+12],%o0
	mov 0,%l0
	cmp %l0,%o0
	bge L131
	cmp %o0,0
	mov %l1,%l2
L46:
	ld [%l2+28],%o1
	cmp %o1,0
	bne L44
	nop
	call _printf,0
	or %l5,%lo(LC30),%o0
	b L132
	ld [%l1+12],%o0
L44:
	call _printf,0
	or %l4,%lo(LC31),%o0
	ld [%l1+12],%o0
L132:
	add %l0,1,%l0
	cmp %l0,%o0
	bl L46
	add %l2,4,%l2
	ld [%l1+12],%o0
	cmp %o0,0
L131:
	bne,a L133
	sethi %hi(LC33),%o0
	sethi %hi(LC32),%o0
	call _printf,0
	or %o0,%lo(LC32),%o0
	sethi %hi(LC33),%o0
L133:
	call _printf,0
	or %o0,%lo(LC33),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne L134
	mov 0,%l3
	b L136
	sethi %hi(LC19),%o0
L29:
	call _printf,0
	or %o0,%lo(LC34),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L136
	sethi %hi(LC19),%o0
	sethi %hi(LC28),%l3
	sethi %hi(LC35),%l2
L58:
	call _printf,0
	or %l3,%lo(LC28),%o0
	ld [%l1+12],%o0
	mov 0,%l0
	cmp %l0,%o0
	bge L137
	cmp %o0,0
	add %l1,%l0,%o0
L138:
	ldsb [%o0+508],%o1
	call _printf,0
	or %l2,%lo(LC35),%o0
	ld [%l1+12],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	bl L138
	add %l1,%l0,%o0
	ld [%l1+12],%o0
	cmp %o0,0
L137:
	bne,a L139
	sethi %hi(LC33),%o0
	sethi %hi(LC32),%o0
	call _printf,0
	or %o0,%lo(LC32),%o0
	sethi %hi(LC33),%o0
L139:
	call _printf,0
	or %o0,%lo(LC33),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne L58
	nop
L51:
	sethi %hi(LC19),%o0
L136:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC36),%o0
	call _printf,0
	or %o0,%lo(LC36),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be,a L140
	sethi %hi(LC19),%o0
	sethi %hi(LC28),%l5
	sethi %hi(_mode_name),%o0
	or %o0,%lo(_mode_name),%l4
	sethi %hi(LC37),%l3
L67:
	call _printf,0
	or %l5,%lo(LC28),%o0
	ld [%l1+12],%o0
	mov 0,%l2
	cmp %l2,%o0
	bge L141
	cmp %o0,0
	mov %l1,%l0
	ld [%l0+548],%o1
L142:
	or %l3,%lo(LC37),%o0
	sll %o1,2,%o1
	ld [%o1+%l4],%o1
	call _printf,0
	add %l0,4,%l0
	ld [%l1+12],%o0
	add %l2,1,%l2
	cmp %l2,%o0
	bl,a L142
	ld [%l0+548],%o1
	ld [%l1+12],%o0
	cmp %o0,0
L141:
	bne,a L143
	sethi %hi(LC33),%o0
	sethi %hi(LC38),%o0
	call _printf,0
	or %o0,%lo(LC38),%o0
	sethi %hi(LC33),%o0
L143:
	call _printf,0
	or %o0,%lo(LC33),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne L67
	sethi %hi(LC19),%o0
L140:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC39),%o0
	call _printf,0
	or %o0,%lo(LC39),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L144
	sethi %hi(LC19),%o0
	sethi %hi(LC28),%l3
	sethi %hi(LC35),%l2
L76:
	call _printf,0
	or %l3,%lo(LC28),%o0
	ld [%l1+12],%o0
	mov 0,%l0
	cmp %l0,%o0
	bge L145
	cmp %o0,0
	add %l1,%l0,%o0
L146:
	ldsb [%o0+708],%o1
	call _printf,0
	or %l2,%lo(LC35),%o0
	ld [%l1+12],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	bl L146
	add %l1,%l0,%o0
	ld [%l1+12],%o0
	cmp %o0,0
L145:
	bne,a L147
	sethi %hi(LC33),%o0
	sethi %hi(LC32),%o0
	call _printf,0
	or %o0,%lo(LC32),%o0
	sethi %hi(LC33),%o0
L147:
	call _printf,0
	or %o0,%lo(LC33),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne L76
	sethi %hi(LC19),%o0
L144:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC40),%o0
	call _printf,0
	or %o0,%lo(LC40),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L148
	sethi %hi(LC19),%o0
	sethi %hi(LC28),%l5
	sethi %hi(LC42),%l4
	sethi %hi(LC41),%l3
L87:
	call _printf,0
	or %l5,%lo(LC28),%o0
	ld [%l1+12],%o0
	mov 0,%l0
	cmp %l0,%o0
	bge L149
	cmp %o0,0
	mov %l1,%l2
L85:
	ld [%l2+348],%o1
	cmp %o1,0
	be,a L150
	or %l4,%lo(LC42),%o1
	ldsb [%o1],%o0
	cmp %o0,0
	be,a L150
	or %l4,%lo(LC42),%o1
L150:
	call _printf,0
	or %l3,%lo(LC41),%o0
	ld [%l1+12],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	bl L85
	add %l2,4,%l2
	ld [%l1+12],%o0
	cmp %o0,0
L149:
	bne,a L151
	sethi %hi(LC33),%o0
	sethi %hi(LC32),%o0
	call _printf,0
	or %o0,%lo(LC32),%o0
	sethi %hi(LC33),%o0
L151:
	call _printf,0
	or %o0,%lo(LC33),%o0
	ld [%l1+24],%l1
	cmp %l1,0
	bne L87
	sethi %hi(LC19),%o0
L148:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC43),%o0
	call _printf,0
	or %o0,%lo(LC43),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L152
	sethi %hi(LC19),%o0
	sethi %hi(LC44),%l2
	sethi %hi(LC45),%l0
	ld [%l1+752],%o1
L154:
	cmp %o1,0
	be L91
	nop
	call _printf,0
	or %l2,%lo(LC44),%o0
	b L153
	ld [%l1+24],%l1
L91:
	call _printf,0
	or %l0,%lo(LC45),%o0
	ld [%l1+24],%l1
L153:
	cmp %l1,0
	bne,a L154
	ld [%l1+752],%o1
	sethi %hi(LC19),%o0
L152:
	call _printf,0
	or %o0,%lo(LC19),%o0
	sethi %hi(LC46),%o0
	call _printf,0
	or %o0,%lo(LC46),%o0
	sethi %hi(_insn_data),%o0
	ld [%o0+%lo(_insn_data)],%l1
	cmp %l1,0
	be L155
	sethi %hi(LC19),%o0
	sethi %hi(LC25),%l2
	sethi %hi(LC47),%l0
	ld [%l1+20],%o1
L157:
	cmp %o1,0
	be L97
	nop
	call _printf,0
	or %l2,%lo(LC25),%o0
	b L156
	ld [%l1+24],%l1
L97:
	call _printf,0
	or %l0,%lo(LC47),%o0
	ld [%l1+24],%l1
L156:
	cmp %l1,0
	bne,a L157
	ld [%l1+20],%o1
	sethi %hi(LC19),%o0
L155:
	call _printf,0
	or %o0,%lo(LC19),%o0
	ret
	restore
