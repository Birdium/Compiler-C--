.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

g:
	lw $t1, 16($sp)
	sw $t1, 0($sp)
	lw $t1, 16($sp)
	sw $t1, 4($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	add $t0, $t1, $t2
	sw $t0, 8($sp)
	lw $v0, 8($sp)
	jr $ra

write_all:
	lw $t1, 112($sp)
	sw $t1, 0($sp)
	lw $a0, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 4($sp)
	lw $t1, 116($sp)
	sw $t1, 8($sp)
	lw $a0, 8($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 12($sp)
	lw $t1, 120($sp)
	sw $t1, 16($sp)
	lw $a0, 16($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 20($sp)
	lw $t1, 124($sp)
	sw $t1, 24($sp)
	lw $a0, 24($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 28($sp)
	lw $t1, 128($sp)
	sw $t1, 32($sp)
	lw $a0, 32($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 36($sp)
	lw $t1, 132($sp)
	sw $t1, 40($sp)
	lw $a0, 40($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 44($sp)
	lw $t1, 136($sp)
	sw $t1, 48($sp)
	lw $a0, 48($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 52($sp)
	lw $t1, 140($sp)
	sw $t1, 56($sp)
	lw $a0, 56($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 60($sp)
	lw $t1, 144($sp)
	sw $t1, 64($sp)
	lw $a0, 64($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 68($sp)
	lw $t1, 148($sp)
	sw $t1, 72($sp)
	lw $a0, 72($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 76($sp)
	lw $t1, 152($sp)
	sw $t1, 80($sp)
	lw $a0, 80($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 84($sp)
	lw $t1, 156($sp)
	sw $t1, 88($sp)
	lw $a0, 88($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 92($sp)
	lw $t1, 160($sp)
	sw $t1, 96($sp)
	lw $a0, 96($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 100($sp)
	li $t1, 1
	sw $t1, 104($sp)
	lw $v0, 104($sp)
	jr $ra

	addi $sp, $sp, -332
main:
	li $t1, 0
	sw $t1, 40($sp)
	lw $t1, 40($sp)
	sw $t1, 44($sp)
	li $t1, 1
	sw $t1, 48($sp)
	lw $t1, 48($sp)
	sw $t1, 52($sp)
	lw $t1, 52($sp)
	sw $t1, 56($sp)
	li $t1, 2
	sw $t1, 60($sp)
	lw $t1, 60($sp)
	sw $t1, 64($sp)
	lw $t1, 64($sp)
	sw $t1, 68($sp)
	li $t1, 3
	sw $t1, 72($sp)
	lw $t1, 72($sp)
	sw $t1, 76($sp)
	lw $t1, 76($sp)
	sw $t1, 80($sp)
label0:
	lw $t1, 44($sp)
	sw $t1, 84($sp)
	li $t1, 10
	sw $t1, 88($sp)
	lw $t1, 84($sp)
	lw $t2, 88($sp)
	blt $t1, $t2, label1
	j label2
label1:
	lw $t1, 44($sp)
	sw $t1, 92($sp)
	lw $t1, 92($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 96($sp)
	addi $t1, $sp, 0
	lw $t2, 96($sp)
	add $t0, $t1, $t2
	sw $t0, 100($sp)
	lw $t1, 44($sp)
	sw $t1, 104($sp)
	lw $t1, 104($sp)
	lw $t2, 100($sp)
	sw $t1, 0($t2)
	lw $t1, 100($sp)
	lw $t1, 0($t1)
	sw $t1, 108($sp)
	lw $t1, 44($sp)
	sw $t1, 112($sp)
	li $t1, 1
	sw $t1, 116($sp)
	lw $t1, 112($sp)
	lw $t2, 116($sp)
	add $t0, $t1, $t2
	sw $t0, 120($sp)
	lw $t1, 120($sp)
	sw $t1, 44($sp)
	lw $t1, 44($sp)
	sw $t1, 124($sp)
	j label0
label2:
	lw $t1, 52($sp)
	sw $t1, 128($sp)
	lw $t1, 64($sp)
	sw $t1, 132($sp)
	lw $t1, 76($sp)
	sw $t1, 136($sp)
	li $t1, 0
	sw $t1, 140($sp)
	lw $t1, 140($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 144($sp)
	addi $t1, $sp, 0
	lw $t2, 144($sp)
	add $t0, $t1, $t2
	sw $t0, 148($sp)
	lw $t1, 148($sp)
	lw $t1, 0($t1)
	sw $t1, 152($sp)
	li $t1, 1
	sw $t1, 156($sp)
	lw $t1, 156($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 160($sp)
	addi $t1, $sp, 0
	lw $t2, 160($sp)
	add $t0, $t1, $t2
	sw $t0, 164($sp)
	lw $t1, 164($sp)
	lw $t1, 0($t1)
	sw $t1, 168($sp)
	lw $t1, 168($sp)
	sw $t1, -4($sp)
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	addi $sp, $sp, -12
	jal g
	addi $sp, $sp, 12
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	sw $v0, 172($sp)
	li $t1, 2
	sw $t1, 176($sp)
	lw $t1, 176($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 180($sp)
	addi $t1, $sp, 0
	lw $t2, 180($sp)
	add $t0, $t1, $t2
	sw $t0, 184($sp)
	lw $t1, 184($sp)
	lw $t1, 0($t1)
	sw $t1, 188($sp)
	li $t1, 3
	sw $t1, 192($sp)
	lw $t1, 192($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 196($sp)
	addi $t1, $sp, 0
	lw $t2, 196($sp)
	add $t0, $t1, $t2
	sw $t0, 200($sp)
	lw $t1, 200($sp)
	lw $t1, 0($t1)
	sw $t1, 204($sp)
	li $t1, 4
	sw $t1, 208($sp)
	lw $t1, 208($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 212($sp)
	addi $t1, $sp, 0
	lw $t2, 212($sp)
	add $t0, $t1, $t2
	sw $t0, 216($sp)
	lw $t1, 216($sp)
	lw $t1, 0($t1)
	sw $t1, 220($sp)
	li $t1, 5
	sw $t1, 224($sp)
	lw $t1, 224($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 228($sp)
	addi $t1, $sp, 0
	lw $t2, 228($sp)
	add $t0, $t1, $t2
	sw $t0, 232($sp)
	lw $t1, 232($sp)
	lw $t1, 0($t1)
	sw $t1, 236($sp)
	li $t1, 6
	sw $t1, 240($sp)
	lw $t1, 240($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 244($sp)
	addi $t1, $sp, 0
	lw $t2, 244($sp)
	add $t0, $t1, $t2
	sw $t0, 248($sp)
	lw $t1, 248($sp)
	lw $t1, 0($t1)
	sw $t1, 252($sp)
	li $t1, 7
	sw $t1, 256($sp)
	lw $t1, 256($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 260($sp)
	addi $t1, $sp, 0
	lw $t2, 260($sp)
	add $t0, $t1, $t2
	sw $t0, 264($sp)
	lw $t1, 264($sp)
	lw $t1, 0($t1)
	sw $t1, 268($sp)
	lw $t1, 76($sp)
	sw $t1, 272($sp)
	lw $t1, 272($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 276($sp)
	addi $t1, $sp, 0
	lw $t2, 276($sp)
	add $t0, $t1, $t2
	sw $t0, 280($sp)
	lw $t1, 280($sp)
	lw $t1, 0($t1)
	sw $t1, 284($sp)
	lw $t1, 76($sp)
	sw $t1, 288($sp)
	lw $t1, 288($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 292($sp)
	addi $t1, $sp, 0
	lw $t2, 292($sp)
	add $t0, $t1, $t2
	sw $t0, 296($sp)
	lw $t1, 296($sp)
	lw $t1, 0($t1)
	sw $t1, 300($sp)
	li $t1, 6
	sw $t1, 304($sp)
	lw $t1, 304($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 308($sp)
	addi $t1, $sp, 0
	lw $t2, 308($sp)
	add $t0, $t1, $t2
	sw $t0, 312($sp)
	lw $t1, 312($sp)
	lw $t1, 0($t1)
	sw $t1, 316($sp)
	lw $t1, 300($sp)
	lw $t2, 316($sp)
	add $t0, $t1, $t2
	sw $t0, 320($sp)
	lw $t1, 320($sp)
	sw $t1, -4($sp)
	lw $t1, 284($sp)
	sw $t1, -8($sp)
	lw $t1, 268($sp)
	sw $t1, -12($sp)
	lw $t1, 252($sp)
	sw $t1, -16($sp)
	lw $t1, 236($sp)
	sw $t1, -20($sp)
	lw $t1, 220($sp)
	sw $t1, -24($sp)
	lw $t1, 204($sp)
	sw $t1, -28($sp)
	lw $t1, 188($sp)
	sw $t1, -32($sp)
	lw $t1, 172($sp)
	sw $t1, -36($sp)
	lw $t1, 152($sp)
	sw $t1, -40($sp)
	lw $t1, 136($sp)
	sw $t1, -44($sp)
	lw $t1, 132($sp)
	sw $t1, -48($sp)
	lw $t1, 128($sp)
	sw $t1, -52($sp)
	addi $sp, $sp, -56
	sw $ra, 0($sp)
	addi $sp, $sp, -108
	jal write_all
	addi $sp, $sp, 108
	lw $ra, 0($sp)
	addi $sp, $sp, 56
	sw $v0, 324($sp)
	li $t1, 0
	sw $t1, 328($sp)
	lw $v0, 328($sp)
	addi $sp, $sp, 332
	jr $ra

