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
	.align 8
LC48:
	.ascii "Too many operands (%d) in one instruction pattern.\12\0"
	.align 4
	.global _scan_operands
	.proc	020
_scan_operands:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	cmp %i0,0
	be L158
	nop
	lduh [%i0],%o2
	cmp %o2,4
	bne L160
	cmp %o2,6
	ld [%i0+4],%l1
	sethi %hi(_max_opno),%o1
	ld [%o1+%lo(_max_opno)],%o0
	cmp %l1,%o0
	bg,a L161
	st %l1,[%o1+%lo(_max_opno)]
L161:
	ld [%o1+%lo(_max_opno)],%o1
	cmp %o1,39
	ble L162
	sll %l1,2,%l0
	sethi %hi(LC48),%o0
	or %o0,%lo(LC48),%o0
	call _error,0
	add %o1,1,%o1
L162:
	sethi %hi(_strict_low),%o0
	ldub [%i0+2],%o1
	or %o0,%lo(_strict_low),%o0
	stb %i2,[%l1+%o0]
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	st %o1,[%l0+%o0]
	sethi %hi(_predicates),%o0
	ld [%i0+8],%o1
	or %o0,%lo(_predicates),%o0
	st %o1,[%l0+%o0]
	sethi %hi(_constraints),%o0
	ld [%i0+12],%o1
	or %o0,%lo(_constraints),%o0
	st %o1,[%l0+%o0]
	ld [%i0+12],%o1
	cmp %o1,0
	be L190
	sethi %hi(_address_p),%o0
	ldsb [%o1],%o0
	cmp %o0,0
	be L190
	sethi %hi(_address_p),%o0
	call _n_occurrences,0
	mov 44,%o0
	sethi %hi(_op_n_alternatives),%o1
	or %o1,%lo(_op_n_alternatives),%o1
	add %o0,1,%o0
	st %o0,[%l0+%o1]
	mov 1,%o0
	sethi %hi(_have_constraints),%o1
	st %o0,[%o1+%lo(_have_constraints)]
	sethi %hi(_address_p),%o0
L190:
	or %o0,%lo(_address_p),%o0
	b L158
	stb %i1,[%l1+%o0]
L160:
	bne L164
	cmp %o2,5
	ld [%i0+4],%l0
	sethi %hi(_max_opno),%o1
	ld [%o1+%lo(_max_opno)],%o0
	cmp %l0,%o0
	bg,a L165
	st %l0,[%o1+%lo(_max_opno)]
L165:
	ld [%o1+%lo(_max_opno)],%o1
	cmp %o1,39
	ble L166
	sethi %hi(LC48),%o0
	or %o0,%lo(LC48),%o0
	call _error,0
	add %o1,1,%o1
L166:
	sethi %hi(_strict_low),%o0
	ldub [%i0+2],%o2
	or %o0,%lo(_strict_low),%o0
	stb %g0,[%l0+%o0]
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	sll %l0,2,%o1
	st %o2,[%o1+%o0]
	sethi %hi(_constraints),%o0
	ld [%i0+8],%o2
	or %o0,%lo(_constraints),%o0
	st %g0,[%o1+%o0]
	sethi %hi(_address_p),%o0
	or %o0,%lo(_address_p),%o0
	stb %g0,[%l0+%o0]
	sethi %hi(_predicates),%o0
	or %o0,%lo(_predicates),%o0
	st %o2,[%o1+%o0]
	ld [%i0+12],%o2
	ld [%o2],%o0
	mov 0,%l0
	cmp %l0,%o0
	bgeu L158
	mov 0,%o1
L191:
	sll %l0,2,%o0
	add %o2,%o0,%o0
	ld [%o0+4],%o0
	call _scan_operands,0
	mov 0,%o2
	ld [%i0+12],%o2
	ld [%o2],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	blu L191
	mov 0,%o1
	b,a L158
L164:
	bne L171
	cmp %o2,12
	sethi %hi(_num_dups),%o1
	ld [%o1+%lo(_num_dups)],%o0
	add %o0,1,%o0
	b L158
	st %o0,[%o1+%lo(_num_dups)]
L171:
	bne L172
	cmp %o2,36
	ld [%i0+4],%o0
	mov 1,%o1
	call _scan_operands,0
	mov 0,%o2
	b,a L158
L172:
	bne L173
	mov 0,%l0
	ld [%i0+4],%o0
	mov 0,%o1
	call _scan_operands,0
	mov 1,%o2
	b,a L158
L173:
	sethi %hi(_rtx_format),%o1
	sll %o2,2,%o2
	sethi %hi(_rtx_length),%o0
	or %o0,%lo(_rtx_length),%o3
	ld [%o2+%o3],%o0
	or %o1,%lo(_rtx_format),%o1
	cmp %l0,%o0
	bge L158
	ld [%o2+%o1],%l1
	mov %o3,%l2
	mov %i0,%i2
L187:
	ldsb [%l1],%o0
	cmp %o0,69
	be L179
	add %l1,1,%l1
	cmp %o0,101
	bne,a L192
	lduh [%i0],%o0
	ld [%i2+4],%o0
	mov 0,%o1
	call _scan_operands,0
	mov 0,%o2
	b L192
	lduh [%i0],%o0
L179:
	ld [%i2+4],%o0
	cmp %o0,0
	be,a L192
	lduh [%i0],%o0
	ld [%o0],%o0
	mov 0,%i1
	cmp %i1,%o0
	bgeu,a L192
	lduh [%i0],%o0
	mov 0,%o1
L193:
	ld [%i2+4],%o0
	sll %i1,2,%o2
	add %o0,%o2,%o0
	ld [%o0+4],%o0
	call _scan_operands,0
	mov 0,%o2
	ld [%i2+4],%o0
	ld [%o0],%o0
	add %i1,1,%i1
	cmp %i1,%o0
	blu L193
	mov 0,%o1
	lduh [%i0],%o0
L192:
	sll %o0,2,%o0
	ld [%o0+%l2],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	bl L187
	add %i2,4,%i2
L158:
	ret
	restore
	.align 8
LC49:
	.ascii "\12char *\12\0"
	.align 8
LC50:
	.ascii "output_%d (operands, insn)\12\0"
	.align 8
LC51:
	.ascii "     rtx *operands;\12\0"
	.align 8
LC52:
	.ascii "     rtx insn;\12\0"
	.align 8
LC53:
	.ascii "{\12\0"
	.align 8
LC54:
	.ascii "}\12\0"
	.align 4
	.global _gen_insn
	.proc	020
_gen_insn:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	call _xmalloc,0
	mov 756,%o0
	sethi %hi(_next_code_number),%o1
	ld [%o1+%lo(_next_code_number)],%o2
	mov %o0,%l1
	st %o2,[%l1]
	add %o2,1,%o2
	ld [%i0+4],%o3
	st %o2,[%o1+%lo(_next_code_number)]
	ldsb [%o3],%o0
	cmp %o0,0
	be,a L196
	st %g0,[%l1+4]
	st %o3,[%l1+4]
L196:
	sethi %hi(_end_of_insn_data),%o0
	ld [%o0+%lo(_end_of_insn_data)],%o0
	cmp %o0,0
	be L197
	st %g0,[%l1+24]
	b L198
	st %l1,[%o0+24]
L197:
	sethi %hi(_insn_data),%o0
	st %l1,[%o0+%lo(_insn_data)]
L198:
	sethi %hi(_end_of_insn_data),%o0
	st %l1,[%o0+%lo(_end_of_insn_data)]
	mov -1,%o0
	sethi %hi(_max_opno),%o1
	st %o0,[%o1+%lo(_max_opno)]
	sethi %hi(_num_dups),%o0
	st %g0,[%o0+%lo(_num_dups)]
	sethi %hi(_constraints),%o0
	or %o0,%lo(_constraints),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_op_n_alternatives),%o0
	or %o0,%lo(_op_n_alternatives),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_predicates),%o0
	or %o0,%lo(_predicates),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_address_p),%o0
	or %o0,%lo(_address_p),%o0
	call _mybzero,0
	mov 40,%o1
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_strict_low),%o0
	or %o0,%lo(_strict_low),%o0
	call _mybzero,0
	mov 40,%o1
	ld [%i0+8],%o2
	ld [%o2],%o0
	mov 0,%l0
	cmp %l0,%o0
	bgeu L220
	sethi %hi(_constraints),%o0
	mov 0,%o1
L221:
	sll %l0,2,%o0
	add %o2,%o0,%o0
	ld [%o0+4],%o0
	call _scan_operands,0
	mov 0,%o2
	ld [%i0+8],%o2
	ld [%o2],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	blu L221
	mov 0,%o1
	sethi %hi(_constraints),%o0
L220:
	or %o0,%lo(_constraints),%o0
	sethi %hi(_max_opno),%o2
	ld [%o2+%lo(_max_opno)],%o4
	add %l1,28,%o1
	mov 160,%o2
	sethi %hi(_num_dups),%o3
	ld [%o3+%lo(_num_dups)],%o3
	add %o4,1,%o4
	st %o4,[%l1+12]
	call _mybcopy,0
	st %o3,[%l1+16]
	sethi %hi(_op_n_alternatives),%o0
	or %o0,%lo(_op_n_alternatives),%o0
	add %l1,188,%o1
	call _mybcopy,0
	mov 160,%o2
	sethi %hi(_predicates),%o0
	or %o0,%lo(_predicates),%o0
	add %l1,348,%o1
	call _mybcopy,0
	mov 160,%o2
	sethi %hi(_address_p),%o0
	or %o0,%lo(_address_p),%o0
	add %l1,508,%o1
	call _mybcopy,0
	mov 40,%o2
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	add %l1,548,%o1
	call _mybcopy,0
	mov 160,%o2
	sethi %hi(_strict_low),%o0
	or %o0,%lo(_strict_low),%o0
	add %l1,708,%o1
	call _mybcopy,0
	mov 40,%o2
	ld [%i0+20],%o0
	st %o0,[%l1+752]
	ld [%i0+16],%o1
	ldsb [%o1],%o0
	cmp %o0,42
	be,a L203
	st %g0,[%l1+8]
	st %o1,[%l1+8]
	b L194
	stb %g0,[%l1+748]
L203:
	mov 1,%o0
	stb %o0,[%l1+748]
	sethi %hi(LC49),%o0
	call _printf,0
	or %o0,%lo(LC49),%o0
	sethi %hi(LC50),%o0
	ld [%l1],%o1
	call _printf,0
	or %o0,%lo(LC50),%o0
	sethi %hi(LC51),%o0
	call _printf,0
	or %o0,%lo(LC51),%o0
	sethi %hi(LC52),%o0
	call _printf,0
	or %o0,%lo(LC52),%o0
	sethi %hi(LC53),%o0
	call _printf,0
	or %o0,%lo(LC53),%o0
	ld [%i0+16],%o0
	ldsb [%o0+1],%o1
	cmp %o1,0
	be L205
	add %o0,1,%l0
	sethi %hi(__iob+20),%l1
	or %l1,%lo(__iob+20),%i0
	ld [%l1+%lo(__iob+20)],%o0
L224:
	add %o0,-1,%o2
	cmp %o2,0
	bl L206
	st %o2,[%l1+%lo(__iob+20)]
	ld [%i0+4],%o0
	add %o0,1,%o1
	st %o1,[%i0+4]
	ldub [%l0],%o1
	add %l0,1,%l0
	b L204
	stb %o1,[%o0]
L206:
	lduh [%i0+16],%o0
	andcc %o0,128,%g0
	be L208
	sub %g0,%o2,%o0
	ld [%i0+12],%o1
	cmp %o0,%o1
	bge,a L222
	ldub [%l0],%o0
	ldub [%l0],%o1
	ld [%i0+4],%o0
	add %l0,1,%l0
	cmp %o1,10
	be L210
	stb %o1,[%o0]
	ld [%i0+4],%o0
	add %o0,1,%o0
	b L204
	st %o0,[%i0+4]
L210:
	ld [%i0+4],%o0
	ldub [%o0],%o0
	call __flsbuf,0
	mov %i0,%o1
	b L223
	ldsb [%l0],%o0
L208:
	ldub [%l0],%o0
L222:
	add %l0,1,%l0
	call __flsbuf,0
	or %l1,%lo(__iob+20),%o1
L204:
	ldsb [%l0],%o0
L223:
	cmp %o0,0
	bne L224
	ld [%l1+%lo(__iob+20)],%o0
L205:
	sethi %hi(__iob+20),%o1
	ld [%o1+%lo(__iob+20)],%o0
	or %o1,%lo(__iob+20),%o3
	add %o0,-1,%o2
	cmp %o2,0
	bl L213
	st %o2,[%o1+%lo(__iob+20)]
	ld [%o3+4],%o0
	add %o0,1,%o1
	st %o1,[%o3+4]
	mov 10,%o1
	b L214
	stb %o1,[%o0]
L213:
	lduh [%o3+16],%o0
	andcc %o0,128,%g0
	be L225
	mov 10,%o0
	ld [%o3+12],%o1
	sub %g0,%o2,%o0
	cmp %o0,%o1
	bge,a L225
	mov 10,%o0
	ld [%o3+4],%o0
	mov 10,%o1
	stb %o1,[%o0]
	ld [%o3+4],%o0
	ldub [%o0],%o0
	call __flsbuf,0
	mov %o3,%o1
	b L226
	sethi %hi(LC54),%o0
L225:
	sethi %hi(__iob+20),%o1
	call __flsbuf,0
	or %o1,%lo(__iob+20),%o1
L214:
	sethi %hi(LC54),%o0
L226:
	call _printf,0
	or %o0,%lo(LC54),%o0
L194:
	ret
	restore
	.align 8
LC55:
	.ascii "%s\12\0"
	.align 4
	.global _gen_peephole
	.proc	020
_gen_peephole:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	call _xmalloc,0
	mov 756,%o0
	mov %o0,%l1
	st %g0,[%l1+4]
	sethi %hi(_next_code_number),%o1
	ld [%o1+%lo(_next_code_number)],%o2
	st %g0,[%l1+24]
	add %o2,1,%o0
	st %o0,[%o1+%lo(_next_code_number)]
	sethi %hi(_end_of_insn_data),%o0
	ld [%o0+%lo(_end_of_insn_data)],%o0
	cmp %o0,0
	be L228
	st %o2,[%l1]
	b L229
	st %l1,[%o0+24]
L228:
	sethi %hi(_insn_data),%o0
	st %l1,[%o0+%lo(_insn_data)]
L229:
	sethi %hi(_end_of_insn_data),%o0
	st %l1,[%o0+%lo(_end_of_insn_data)]
	mov -1,%o0
	sethi %hi(_max_opno),%o1
	st %o0,[%o1+%lo(_max_opno)]
	sethi %hi(_constraints),%o0
	or %o0,%lo(_constraints),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_op_n_alternatives),%o0
	or %o0,%lo(_op_n_alternatives),%o0
	call _mybzero,0
	mov 160,%o1
	ld [%i0+4],%o2
	ld [%o2],%o0
	mov 0,%l0
	cmp %l0,%o0
	bgeu,a L236
	st %g0,[%l1+16]
	mov 0,%o1
L237:
	sll %l0,2,%o0
	add %o2,%o0,%o0
	ld [%o0+4],%o0
	call _scan_operands,0
	mov 0,%o2
	ld [%i0+4],%o2
	ld [%o2],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	blu,a L237
	mov 0,%o1
	st %g0,[%l1+16]
L236:
	sethi %hi(_constraints),%o0
	or %o0,%lo(_constraints),%o0
	sethi %hi(_max_opno),%o2
	ld [%o2+%lo(_max_opno)],%o3
	add %l1,28,%o1
	mov 160,%o2
	add %o3,1,%o3
	call _mybcopy,0
	st %o3,[%l1+12]
	sethi %hi(_op_n_alternatives),%o0
	or %o0,%lo(_op_n_alternatives),%o0
	add %l1,188,%o1
	call _mybcopy,0
	mov 160,%o2
	add %l1,348,%o0
	call _mybzero,0
	mov 160,%o1
	add %l1,508,%o0
	call _mybzero,0
	mov 40,%o1
	add %l1,548,%o0
	call _mybzero,0
	mov 160,%o1
	add %l1,708,%o0
	call _mybzero,0
	mov 40,%o1
	ld [%i0+16],%o0
	st %o0,[%l1+752]
	ld [%i0+12],%o1
	ldsb [%o1],%o0
	cmp %o0,42
	be,a L234
	st %g0,[%l1+8]
	st %o1,[%l1+8]
	b L227
	stb %g0,[%l1+748]
L234:
	mov 1,%o0
	stb %o0,[%l1+748]
	sethi %hi(LC49),%o0
	call _printf,0
	or %o0,%lo(LC49),%o0
	sethi %hi(LC50),%o0
	ld [%l1],%o1
	call _printf,0
	or %o0,%lo(LC50),%o0
	sethi %hi(LC51),%o0
	call _printf,0
	or %o0,%lo(LC51),%o0
	sethi %hi(LC52),%o0
	call _printf,0
	or %o0,%lo(LC52),%o0
	sethi %hi(LC53),%o0
	call _printf,0
	or %o0,%lo(LC53),%o0
	sethi %hi(LC55),%o0
	ld [%i0+12],%o1
	or %o0,%lo(LC55),%o0
	call _printf,0
	add %o1,1,%o1
	sethi %hi(LC54),%o0
	call _printf,0
	or %o0,%lo(LC54),%o0
L227:
	ret
	restore
	.align 4
	.global _gen_expand
	.proc	020
_gen_expand:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	call _xmalloc,0
	mov 756,%o0
	sethi %hi(_next_code_number),%o1
	ld [%o1+%lo(_next_code_number)],%o2
	mov %o0,%l1
	st %o2,[%l1]
	add %o2,1,%o2
	ld [%i0+4],%o3
	st %o2,[%o1+%lo(_next_code_number)]
	ldsb [%o3],%o0
	cmp %o0,0
	be,a L240
	st %g0,[%l1+4]
	st %o3,[%l1+4]
L240:
	sethi %hi(_end_of_insn_data),%o0
	ld [%o0+%lo(_end_of_insn_data)],%o0
	cmp %o0,0
	be L241
	st %g0,[%l1+24]
	b L242
	st %l1,[%o0+24]
L241:
	sethi %hi(_insn_data),%o0
	st %l1,[%o0+%lo(_insn_data)]
L242:
	sethi %hi(_end_of_insn_data),%o0
	st %l1,[%o0+%lo(_end_of_insn_data)]
	mov -1,%o0
	sethi %hi(_max_opno),%o1
	st %o0,[%o1+%lo(_max_opno)]
	sethi %hi(_num_dups),%o0
	st %g0,[%o0+%lo(_num_dups)]
	sethi %hi(_predicates),%o0
	or %o0,%lo(_predicates),%o0
	call _mybzero,0
	mov 160,%o1
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	call _mybzero,0
	mov 160,%o1
	ld [%i0+8],%o2
	cmp %o2,0
	be L249
	sethi %hi(_predicates),%o0
	ld [%o2],%o0
	mov 0,%l0
	cmp %l0,%o0
	bgeu L249
	sethi %hi(_predicates),%o0
	mov 0,%o1
L250:
	sll %l0,2,%o0
	add %o2,%o0,%o0
	ld [%o0+4],%o0
	call _scan_operands,0
	mov 0,%o2
	ld [%i0+8],%o2
	ld [%o2],%o0
	add %l0,1,%l0
	cmp %l0,%o0
	blu,a L250
	mov 0,%o1
	sethi %hi(_predicates),%o0
L249:
	or %o0,%lo(_predicates),%o0
	sethi %hi(_max_opno),%o2
	ld [%o2+%lo(_max_opno)],%o3
	add %l1,348,%o1
	mov 160,%o2
	add %o3,1,%o3
	call _mybcopy,0
	st %o3,[%l1+12]
	sethi %hi(_modes),%o0
	or %o0,%lo(_modes),%o0
	add %l1,548,%o1
	call _mybcopy,0
	mov 160,%o2
	add %l1,28,%o0
	call _mybzero,0
	mov 160,%o1
	add %l1,188,%o0
	call _mybzero,0
	mov 160,%o1
	add %l1,508,%o0
	call _mybzero,0
	mov 40,%o1
	add %l1,708,%o0
	call _mybzero,0
	mov 40,%o1
	st %g0,[%l1+16]
	st %g0,[%l1+8]
	stb %g0,[%l1+748]
	st %g0,[%l1+752]
	ret
	restore
	.align 8
LC56:
	.ascii "virtual memory exhausted\0"
	.align 4
	.global _xmalloc
	.proc	04
_xmalloc:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	call _malloc,0
	mov %i0,%o0
	orcc %o0,%g0,%i0
	bne L254
	sethi %hi(LC56),%o0
	call _fatal,0
	or %o0,%lo(LC56),%o0
L254:
	ret
	restore
	.align 4
	.global _xrealloc
	.proc	04
_xrealloc:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	mov %i0,%o0
	call _realloc,0
	mov %i1,%o1
	orcc %o0,%g0,%i0
	bne L258
	sethi %hi(LC56),%o0
	call _fatal,0
	or %o0,%lo(LC56),%o0
L258:
	ret
	restore
	.align 4
	.global _mybzero
	.proc	020
_mybzero:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	b L269
	mov %i1,%g2
L266:
	stb %g0,[%i0]
	add %i0,1,%i0
	mov %i1,%g2
L269:
	cmp %g2,0
	bg L266
	add %i1,-1,%i1
	ret
	restore
	.align 4
	.global _mybcopy
	.proc	020
_mybcopy:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	orcc %i2,%g0,%g2
	ble L276
	add %i2,-1,%i2
L277:
	ldub [%i0],%g3
	add %i0,1,%i0
	mov %i2,%g2
	add %i2,-1,%i2
	cmp %g2,0
	stb %g3,[%i1]
	bg L277
	add %i1,1,%i1
L276:
	ret
	restore
	.align 8
LC57:
	.ascii "genoutput: \0"
	.align 8
LC58:
	.ascii "\12\0"
	.align 4
	.global _fatal
	.proc	020
_fatal:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	sethi %hi(__iob+40),%l0
	or %l0,%lo(__iob+40),%o0
	sethi %hi(LC57),%o1
	call _fprintf,0
	or %o1,%lo(LC57),%o1
	or %l0,%lo(__iob+40),%o0
	mov %i0,%o1
	mov %i1,%o2
	call _fprintf,0
	mov %i2,%o3
	or %l0,%lo(__iob+40),%o0
	sethi %hi(LC58),%o1
	call _fprintf,0
	or %o1,%lo(LC58),%o1
	call _exit,0
	mov 33,%o0
	ret
	restore
	.align 4
	.global _error
	.proc	020
_error:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	sethi %hi(__iob+40),%l0
	or %l0,%lo(__iob+40),%o0
	sethi %hi(LC57),%o1
	call _fprintf,0
	or %o1,%lo(LC57),%o1
	or %l0,%lo(__iob+40),%o0
	mov %i0,%o1
	mov %i1,%o2
	call _fprintf,0
	mov %i2,%o3
	or %l0,%lo(__iob+40),%o0
	sethi %hi(LC58),%o1
	call _fprintf,0
	or %o1,%lo(LC58),%o1
	ret
	restore
	.align 8
LC59:
	.ascii "No input file name.\0"
	.align 8
LC60:
	.ascii "r\0"
	.align 4
	.global _main
	.proc	04
_main:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	call ___main,0
	nop
	sethi %hi(_rtl_obstack),%o0
	ld [%o0+%lo(_rtl_obstack)],%o0
	mov 0,%o1
	mov 0,%o2
	sethi %hi(_xmalloc),%o3
	or %o3,%lo(_xmalloc),%o3
	sethi %hi(_free),%o4
	call __obstack_begin,0
	or %o4,%lo(_free),%o4
	cmp %i0,1
	bg,a L295
	ld [%i1+4],%o0
	sethi %hi(LC59),%o0
	call _fatal,0
	or %o0,%lo(LC59),%o0
	ld [%i1+4],%o0
L295:
	sethi %hi(LC60),%o1
	call _fopen,0
	or %o1,%lo(LC60),%o1
	orcc %o0,%g0,%i0
	bne L285
	nop
	call _perror,0
	ld [%i1+4],%o0
	call _exit,0
	mov 33,%o0
L285:
	call _init_rtl,0
	nop
	call _output_prologue,0
	nop
	sethi %hi(_next_code_number),%o0
	st %g0,[%o0+%lo(_next_code_number)]
	sethi %hi(_have_constraints),%o0
	st %g0,[%o0+%lo(_have_constraints)]
L286:
	call _read_skip_spaces,0
	mov %i0,%o0
	cmp %o0,-1
	be L287
	nop
	call _ungetc,0
	mov %i0,%o1
	call _read_rtx,0
	mov %i0,%o0
	mov %o0,%l0
	lduh [%l0],%o0
	cmp %o0,7
	bne L296
	cmp %o0,8
	call _gen_insn,0
	mov %l0,%o0
	lduh [%l0],%o0
	cmp %o0,8
L296:
	bne,a L297
	lduh [%l0],%o0
	call _gen_peephole,0
	mov %l0,%o0
	lduh [%l0],%o0
L297:
	cmp %o0,10
	bne L286
	nop
	call _gen_expand,0
	mov %l0,%o0
	b,a L286
L287:
	call _output_epilogue,0
	sethi %hi(__iob+20),%l0
	call _fflush,0
	or %l0,%lo(__iob+20),%o0
	or %l0,%lo(__iob+20),%l0
	lduh [%l0+16],%o0
	and %o0,32,%o0
	subcc %g0,%o0,%g0
	subx %g0,0,%o0
	call _exit,0
	and %o0,33,%o0
	ret
	restore
	.align 4
	.global _n_occurrences
	.proc	04
_n_occurrences:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	mov %i0,%i2
	ldsb [%i1],%g2
	mov 0,%i0
	cmp %g2,0
	be L306
	ldub [%i1],%g3
	sll %i2,24,%g2
	sra %g2,24,%i2
L309:
	sll %g3,24,%g2
	sra %g2,24,%g2
	cmp %g2,%i2
	bne L305
	add %i1,1,%i1
	add %i0,1,%i0
L305:
	ldsb [%i1],%g2
	cmp %g2,0
	bne L309
	ldub [%i1],%g3
L306:
	ret
	restore
	.global _obstack
	.common _obstack,40,"bss"
	.global _next_code_number
	.common _next_code_number,8,"bss"
	.global _insn_data
	.common _insn_data,8,"bss"
	.global _end_of_insn_data
	.common _end_of_insn_data,8,"bss"
	.global _have_constraints
	.common _have_constraints,8,"bss"
	.global _max_opno
	.common _max_opno,8,"bss"
	.global _num_dups
	.common _num_dups,8,"bss"
	.global _constraints
	.common _constraints,160,"bss"
	.global _op_n_alternatives
	.common _op_n_alternatives,160,"bss"
	.global _predicates
	.common _predicates,160,"bss"
	.global _address_p
	.common _address_p,40,"bss"
	.global _modes
	.common _modes,160,"bss"
	.global _strict_low
	.common _strict_low,40,"bss"
