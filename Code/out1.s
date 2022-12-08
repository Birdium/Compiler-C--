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

mod:
	lw $t1, 32($sp)
	sw $t1, 0($sp)
	lw $t1, 32($sp)
	sw $t1, 4($sp)
	lw $t1, 36($sp)
	sw $t1, 8($sp)
	lw $t1, 4($sp)
	lw $t2, 8($sp)
	div $t1, $t2
	mflo $t0
	sw $t0, 12($sp)
	lw $t1, 36($sp)
	sw $t1, 16($sp)
	lw $t1, 12($sp)
	lw $t2, 16($sp)
	mul $t0, $t1, $t2
	sw $t0, 20($sp)
	lw $t1, 0($sp)
	lw $t2, 20($sp)
	sub $t0, $t1, $t2
	sw $t0, 24($sp)
	lw $v0, 24($sp)
	jr $ra

do_work:
	lw $t1, 320($sp)
	sw $t1, 0($sp)
	li $t1, 5
	sw $t1, 4($sp)
	lw $t1, 4($sp)
	sw $t1, -4($sp)
	lw $t1, 0($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 8($sp)
	lw $t1, 8($sp)
	sw $t1, 320($sp)
	lw $t1, 320($sp)
	sw $t1, 16($sp)
	li $t1, 2
	sw $t1, 20($sp)
	lw $t1, 320($sp)
	sw $t1, 24($sp)
	lw $t1, 20($sp)
	lw $t2, 24($sp)
	mul $t0, $t1, $t2
	sw $t0, 28($sp)
	lw $t1, 28($sp)
	sw $t1, 32($sp)
	lw $t1, 32($sp)
	sw $t1, 36($sp)
	lw $t1, 320($sp)
	sw $t1, 40($sp)
	lw $t1, 320($sp)
	sw $t1, 44($sp)
	lw $t1, 40($sp)
	lw $t2, 44($sp)
	add $t0, $t1, $t2
	sw $t0, 48($sp)
	lw $t1, 48($sp)
	sw $t1, 52($sp)
	lw $t1, 52($sp)
	sw $t1, 56($sp)
	li $t1, 4
	sw $t1, 60($sp)
	li $t1, 4
	sw $t1, 64($sp)
	lw $t1, 60($sp)
	lw $t2, 64($sp)
	mul $t0, $t1, $t2
	sw $t0, 68($sp)
	lw $t1, 68($sp)
	sw $t1, 72($sp)
	lw $t1, 72($sp)
	sw $t1, 76($sp)
	lw $t1, 72($sp)
	sw $t1, 80($sp)
	lw $t1, 52($sp)
	sw $t1, 84($sp)
	lw $t1, 80($sp)
	lw $t2, 84($sp)
	add $t0, $t1, $t2
	sw $t0, 88($sp)
	lw $t1, 32($sp)
	sw $t1, 92($sp)
	lw $t1, 88($sp)
	lw $t2, 92($sp)
	add $t0, $t1, $t2
	sw $t0, 96($sp)
	lw $t1, 96($sp)
	sw $t1, 32($sp)
	lw $t1, 32($sp)
	sw $t1, 100($sp)
	lw $t1, 32($sp)
	sw $t1, 104($sp)
	lw $t1, 32($sp)
	sw $t1, 108($sp)
	li $t1, 1
	sw $t1, 112($sp)
	lw $t1, 108($sp)
	lw $t2, 112($sp)
	add $t0, $t1, $t2
	sw $t0, 116($sp)
	lw $t1, 52($sp)
	sw $t1, 120($sp)
	li $t1, 1
	sw $t1, 124($sp)
	lw $t1, 120($sp)
	lw $t2, 124($sp)
	add $t0, $t1, $t2
	sw $t0, 128($sp)
	lw $t1, 116($sp)
	lw $t2, 128($sp)
	mul $t0, $t1, $t2
	sw $t0, 132($sp)
	lw $t1, 72($sp)
	sw $t1, 136($sp)
	li $t1, 1
	sw $t1, 140($sp)
	lw $t1, 136($sp)
	lw $t2, 140($sp)
	add $t0, $t1, $t2
	sw $t0, 144($sp)
	lw $t1, 132($sp)
	lw $t2, 144($sp)
	mul $t0, $t1, $t2
	sw $t0, 148($sp)
	lw $t1, 104($sp)
	lw $t2, 148($sp)
	add $t0, $t1, $t2
	sw $t0, 152($sp)
	lw $t1, 152($sp)
	sw $t1, 52($sp)
	lw $t1, 52($sp)
	sw $t1, 156($sp)
	lw $t1, 32($sp)
	sw $t1, 160($sp)
	lw $t1, 32($sp)
	sw $t1, 164($sp)
	li $t1, 1
	sw $t1, 168($sp)
	lw $t1, 164($sp)
	lw $t2, 168($sp)
	add $t0, $t1, $t2
	sw $t0, 172($sp)
	lw $t1, 160($sp)
	lw $t2, 172($sp)
	mul $t0, $t1, $t2
	sw $t0, 176($sp)
	lw $t1, 52($sp)
	sw $t1, 180($sp)
	li $t1, 1
	sw $t1, 184($sp)
	lw $t1, 180($sp)
	lw $t2, 184($sp)
	add $t0, $t1, $t2
	sw $t0, 188($sp)
	lw $t1, 52($sp)
	sw $t1, 192($sp)
	li $t1, 1
	sw $t1, 196($sp)
	lw $t1, 192($sp)
	lw $t2, 196($sp)
	add $t0, $t1, $t2
	sw $t0, 200($sp)
	lw $t1, 188($sp)
	lw $t2, 200($sp)
	mul $t0, $t1, $t2
	sw $t0, 204($sp)
	lw $t1, 176($sp)
	lw $t2, 204($sp)
	add $t0, $t1, $t2
	sw $t0, 208($sp)
	lw $t1, 72($sp)
	sw $t1, 212($sp)
	li $t1, 1
	sw $t1, 216($sp)
	lw $t1, 212($sp)
	lw $t2, 216($sp)
	add $t0, $t1, $t2
	sw $t0, 220($sp)
	lw $t1, 72($sp)
	sw $t1, 224($sp)
	li $t1, 1
	sw $t1, 228($sp)
	lw $t1, 224($sp)
	lw $t2, 228($sp)
	add $t0, $t1, $t2
	sw $t0, 232($sp)
	lw $t1, 220($sp)
	lw $t2, 232($sp)
	mul $t0, $t1, $t2
	sw $t0, 236($sp)
	lw $t1, 208($sp)
	lw $t2, 236($sp)
	add $t0, $t1, $t2
	sw $t0, 240($sp)
	lw $t1, 240($sp)
	sw $t1, 72($sp)
	lw $t1, 72($sp)
	sw $t1, 244($sp)
	lw $t1, 32($sp)
	sw $t1, 248($sp)
	li $t1, 1
	sw $t1, 252($sp)
	lw $t1, 248($sp)
	lw $t2, 252($sp)
	add $t0, $t1, $t2
	sw $t0, 256($sp)
	lw $t1, 52($sp)
	sw $t1, 260($sp)
	li $t1, 1
	sw $t1, 264($sp)
	lw $t1, 260($sp)
	lw $t2, 264($sp)
	add $t0, $t1, $t2
	sw $t0, 268($sp)
	lw $t1, 256($sp)
	lw $t2, 268($sp)
	add $t0, $t1, $t2
	sw $t0, 272($sp)
	lw $t1, 72($sp)
	sw $t1, 276($sp)
	li $t1, 1
	sw $t1, 280($sp)
	lw $t1, 276($sp)
	lw $t2, 280($sp)
	add $t0, $t1, $t2
	sw $t0, 284($sp)
	lw $t1, 272($sp)
	lw $t2, 284($sp)
	add $t0, $t1, $t2
	sw $t0, 288($sp)
	lw $t1, 288($sp)
	sw $t1, 32($sp)
	lw $t1, 32($sp)
	sw $t1, 292($sp)
	lw $t1, 32($sp)
	sw $t1, 296($sp)
	lw $t1, 52($sp)
	sw $t1, 300($sp)
	lw $t1, 296($sp)
	lw $t2, 300($sp)
	add $t0, $t1, $t2
	sw $t0, 304($sp)
	lw $t1, 72($sp)
	sw $t1, 308($sp)
	lw $t1, 304($sp)
	lw $t2, 308($sp)
	add $t0, $t1, $t2
	sw $t0, 312($sp)
	lw $v0, 312($sp)
	jr $ra

	addi $sp, $sp, -1116
main:
	li $t1, 2
	sw $t1, 0($sp)
	lw $t1, 0($sp)
	sw $t1, 4($sp)
	li $t1, 5
	sw $t1, 8($sp)
	lw $t1, 8($sp)
	sw $t1, 12($sp)
	li $t1, 10
	sw $t1, 16($sp)
	lw $t1, 16($sp)
	sw $t1, 20($sp)
	li $t1, 3
	sw $t1, 24($sp)
	lw $t1, 12($sp)
	sw $t1, 28($sp)
	lw $t1, 20($sp)
	sw $t1, 32($sp)
	lw $t1, 28($sp)
	lw $t2, 32($sp)
	mul $t0, $t1, $t2
	sw $t0, 36($sp)
	lw $t1, 24($sp)
	lw $t2, 36($sp)
	mul $t0, $t1, $t2
	sw $t0, 40($sp)
	li $t1, 100
	sw $t1, 44($sp)
	li $t1, 5
	sw $t1, 48($sp)
	lw $t1, 44($sp)
	lw $t2, 48($sp)
	div $t1, $t2
	mflo $t0
	sw $t0, 52($sp)
	lw $t1, 40($sp)
	lw $t2, 52($sp)
	sub $t0, $t1, $t2
	sw $t0, 56($sp)
	lw $t1, 56($sp)
	sw $t1, 60($sp)
	li $t1, 42
	sw $t1, 64($sp)
	lw $t1, 4($sp)
	sw $t1, 68($sp)
	lw $t1, 12($sp)
	sw $t1, 72($sp)
	lw $t1, 68($sp)
	lw $t2, 72($sp)
	mul $t0, $t1, $t2
	sw $t0, 76($sp)
	lw $t1, 4($sp)
	sw $t1, 80($sp)
	lw $t1, 12($sp)
	sw $t1, 84($sp)
	lw $t1, 20($sp)
	sw $t1, 88($sp)
	lw $t1, 84($sp)
	lw $t2, 88($sp)
	mul $t0, $t1, $t2
	sw $t0, 92($sp)
	lw $t1, 80($sp)
	lw $t2, 92($sp)
	mul $t0, $t1, $t2
	sw $t0, 96($sp)
	li $t1, 32
	sw $t1, 100($sp)
	lw $t1, 96($sp)
	lw $t2, 100($sp)
	div $t1, $t2
	mflo $t0
	sw $t0, 104($sp)
	lw $t1, 76($sp)
	lw $t2, 104($sp)
	mul $t0, $t1, $t2
	sw $t0, 108($sp)
	lw $t1, 64($sp)
	lw $t2, 108($sp)
	sub $t0, $t1, $t2
	sw $t0, 112($sp)
	li $t1, 100
	sw $t1, 116($sp)
	lw $t1, 112($sp)
	lw $t2, 116($sp)
	add $t0, $t1, $t2
	sw $t0, 120($sp)
	lw $t1, 120($sp)
	sw $t1, 124($sp)
	li $t1, 3
	sw $t1, 128($sp)
	li $t1, 4
	sw $t1, 132($sp)
	lw $t1, 128($sp)
	lw $t2, 132($sp)
	mul $t0, $t1, $t2
	sw $t0, 136($sp)
	li $t1, 5
	sw $t1, 140($sp)
	lw $t1, 136($sp)
	lw $t2, 140($sp)
	mul $t0, $t1, $t2
	sw $t0, 144($sp)
	li $t1, 10
	sw $t1, 148($sp)
	lw $t1, 144($sp)
	lw $t2, 148($sp)
	sub $t0, $t1, $t2
	sw $t0, 152($sp)
	lw $t1, 4($sp)
	sw $t1, 156($sp)
	lw $t1, 12($sp)
	sw $t1, 160($sp)
	lw $t1, 156($sp)
	lw $t2, 160($sp)
	mul $t0, $t1, $t2
	sw $t0, 164($sp)
	li $t1, 3
	sw $t1, 168($sp)
	lw $t1, 164($sp)
	lw $t2, 168($sp)
	mul $t0, $t1, $t2
	sw $t0, 172($sp)
	li $t1, 0
	lw $t2, 172($sp)
	sub $t0, $t1, $t2
	sw $t0, 176($sp)
	lw $t1, 152($sp)
	lw $t2, 176($sp)
	sub $t0, $t1, $t2
	sw $t0, 180($sp)
	lw $t1, 4($sp)
	sw $t1, 184($sp)
	lw $t1, 12($sp)
	sw $t1, 188($sp)
	lw $t1, 184($sp)
	lw $t2, 188($sp)
	mul $t0, $t1, $t2
	sw $t0, 192($sp)
	lw $t1, 180($sp)
	lw $t2, 192($sp)
	sub $t0, $t1, $t2
	sw $t0, 196($sp)
	lw $t1, 4($sp)
	sw $t1, 200($sp)
	lw $t1, 12($sp)
	sw $t1, 204($sp)
	lw $t1, 200($sp)
	lw $t2, 204($sp)
	mul $t0, $t1, $t2
	sw $t0, 208($sp)
	lw $t1, 196($sp)
	lw $t2, 208($sp)
	sub $t0, $t1, $t2
	sw $t0, 212($sp)
	li $t1, 3
	sw $t1, 216($sp)
	lw $t1, 212($sp)
	lw $t2, 216($sp)
	add $t0, $t1, $t2
	sw $t0, 220($sp)
	li $t1, 2
	sw $t1, 224($sp)
	lw $t1, 220($sp)
	lw $t2, 224($sp)
	add $t0, $t1, $t2
	sw $t0, 228($sp)
	li $t1, 1
	sw $t1, 232($sp)
	lw $t1, 228($sp)
	lw $t2, 232($sp)
	add $t0, $t1, $t2
	sw $t0, 236($sp)
	lw $t1, 236($sp)
	sw $t1, 240($sp)
	li $t1, 0
	sw $t1, 244($sp)
	lw $t1, 244($sp)
	sw $t1, 248($sp)
	li $t1, 0
	sw $t1, 252($sp)
	lw $t1, 252($sp)
	sw $t1, 256($sp)
	lw $t1, 4($sp)
	sw $t1, 260($sp)
	lw $t1, 12($sp)
	sw $t1, 264($sp)
	lw $t1, 260($sp)
	lw $t2, 264($sp)
	mul $t0, $t1, $t2
	sw $t0, 268($sp)
	lw $t1, 268($sp)
	sw $t1, 272($sp)
label0:
	lw $t1, 248($sp)
	sw $t1, 356($sp)
	lw $t1, 240($sp)
	sw $t1, 360($sp)
	lw $t1, 356($sp)
	lw $t2, 360($sp)
	blt $t1, $t2, label1
	j label2
label1:
	lw $t1, 256($sp)
	sw $t1, 364($sp)
	li $t1, 1
	sw $t1, 368($sp)
	lw $t1, 364($sp)
	lw $t2, 368($sp)
	add $t0, $t1, $t2
	sw $t0, 372($sp)
	lw $t1, 372($sp)
	sw $t1, 256($sp)
	lw $t1, 256($sp)
	sw $t1, 376($sp)
	lw $t1, 248($sp)
	sw $t1, 380($sp)
	lw $t1, 272($sp)
	sw $t1, 384($sp)
	lw $t1, 384($sp)
	sw $t1, -4($sp)
	lw $t1, 380($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 388($sp)
	lw $t1, 388($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 392($sp)
	addi $t1, $sp, 276
	lw $t2, 392($sp)
	add $t0, $t1, $t2
	sw $t0, 396($sp)
	lw $t1, 60($sp)
	sw $t1, 400($sp)
	lw $t1, 400($sp)
	lw $t2, 396($sp)
	sw $t1, 0($t2)
	lw $t1, 396($sp)
	lw $t1, 0($t1)
	sw $t1, 408($sp)
	lw $t1, 60($sp)
	sw $t1, 412($sp)
	li $t1, 1
	sw $t1, 416($sp)
	lw $t1, 412($sp)
	lw $t2, 416($sp)
	add $t0, $t1, $t2
	sw $t0, 420($sp)
	lw $t1, 420($sp)
	sw $t1, 60($sp)
	lw $t1, 60($sp)
	sw $t1, 424($sp)
	lw $t1, 248($sp)
	sw $t1, 428($sp)
	li $t1, 1
	sw $t1, 432($sp)
	lw $t1, 428($sp)
	lw $t2, 432($sp)
	add $t0, $t1, $t2
	sw $t0, 436($sp)
	lw $t1, 436($sp)
	sw $t1, 248($sp)
	lw $t1, 248($sp)
	sw $t1, 440($sp)
	j label0
label2:
	li $t1, 0
	sw $t1, 444($sp)
	lw $t1, 444($sp)
	sw $t1, 248($sp)
	lw $t1, 248($sp)
	sw $t1, 448($sp)
label3:
	lw $t1, 248($sp)
	sw $t1, 452($sp)
	li $t1, 100
	sw $t1, 456($sp)
	lw $t1, 452($sp)
	lw $t2, 456($sp)
	blt $t1, $t2, label4
	j label5
label4:
	lw $t1, 248($sp)
	sw $t1, 460($sp)
	lw $t1, 272($sp)
	sw $t1, 464($sp)
	lw $t1, 464($sp)
	sw $t1, -4($sp)
	lw $t1, 460($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 468($sp)
	lw $t1, 468($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 472($sp)
	addi $t1, $sp, 316
	lw $t2, 472($sp)
	add $t0, $t1, $t2
	sw $t0, 476($sp)
	lw $t1, 60($sp)
	sw $t1, 480($sp)
	lw $t1, 480($sp)
	lw $t2, 476($sp)
	sw $t1, 0($t2)
	lw $t1, 476($sp)
	lw $t1, 0($t1)
	sw $t1, 488($sp)
	lw $t1, 60($sp)
	sw $t1, 492($sp)
	li $t1, 1
	sw $t1, 496($sp)
	lw $t1, 492($sp)
	lw $t2, 496($sp)
	add $t0, $t1, $t2
	sw $t0, 500($sp)
	lw $t1, 500($sp)
	sw $t1, 60($sp)
	lw $t1, 60($sp)
	sw $t1, 504($sp)
	lw $t1, 248($sp)
	sw $t1, 508($sp)
	li $t1, 1
	sw $t1, 512($sp)
	lw $t1, 508($sp)
	lw $t2, 512($sp)
	add $t0, $t1, $t2
	sw $t0, 516($sp)
	lw $t1, 516($sp)
	sw $t1, 248($sp)
	lw $t1, 248($sp)
	sw $t1, 520($sp)
	j label3
label5:
	li $t1, 0
	sw $t1, 524($sp)
	lw $t1, 524($sp)
	sw $t1, 248($sp)
	lw $t1, 248($sp)
	sw $t1, 528($sp)
label6:
	lw $t1, 248($sp)
	sw $t1, 532($sp)
	li $t1, 10
	sw $t1, 536($sp)
	lw $t1, 536($sp)
	sw $t1, -4($sp)
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	addi $sp, $sp, -316
	jal do_work
	addi $sp, $sp, 316
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	sw $v0, 540($sp)
	li $t1, 30
	sw $t1, 544($sp)
	lw $t1, 544($sp)
	sw $t1, -4($sp)
	lw $t1, 540($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 548($sp)
	lw $t1, 532($sp)
	lw $t2, 548($sp)
	blt $t1, $t2, label7
	j label8
label7:
	lw $t1, 248($sp)
	sw $t1, 552($sp)
	lw $t1, 272($sp)
	sw $t1, 556($sp)
	lw $t1, 556($sp)
	sw $t1, -4($sp)
	lw $t1, 552($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 560($sp)
	lw $t1, 560($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 564($sp)
	addi $t1, $sp, 276
	lw $t2, 564($sp)
	add $t0, $t1, $t2
	sw $t0, 568($sp)
	lw $t1, 568($sp)
	lw $t1, 0($t1)
	sw $t1, 572($sp)
	lw $t1, 572($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 576($sp)
	lw $t1, 256($sp)
	sw $t1, 580($sp)
	li $t1, 1
	sw $t1, 584($sp)
	li $t1, 0
	lw $t2, 584($sp)
	sub $t0, $t1, $t2
	sw $t0, 588($sp)
	lw $t1, 248($sp)
	sw $t1, 592($sp)
	lw $t1, 588($sp)
	lw $t2, 592($sp)
	mul $t0, $t1, $t2
	sw $t0, 596($sp)
	lw $t1, 580($sp)
	lw $t2, 596($sp)
	add $t0, $t1, $t2
	sw $t0, 600($sp)
	lw $t1, 124($sp)
	sw $t1, 604($sp)
	lw $t1, 604($sp)
	sw $t1, -4($sp)
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	addi $sp, $sp, -316
	jal do_work
	addi $sp, $sp, 316
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	sw $v0, 608($sp)
	lw $t1, 600($sp)
	lw $t2, 608($sp)
	add $t0, $t1, $t2
	sw $t0, 612($sp)
	lw $t1, 612($sp)
	sw $t1, 256($sp)
	lw $t1, 256($sp)
	sw $t1, 616($sp)
	lw $t1, 124($sp)
	sw $t1, 620($sp)
	li $t1, 2
	sw $t1, 624($sp)
	lw $t1, 248($sp)
	sw $t1, 628($sp)
	lw $t1, 624($sp)
	lw $t2, 628($sp)
	mul $t0, $t1, $t2
	sw $t0, 632($sp)
	lw $t1, 620($sp)
	lw $t2, 632($sp)
	add $t0, $t1, $t2
	sw $t0, 636($sp)
	lw $t1, 636($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 640($sp)
	lw $t1, 124($sp)
	sw $t1, 644($sp)
	li $t1, 2
	sw $t1, 648($sp)
	lw $t1, 248($sp)
	sw $t1, 652($sp)
	lw $t1, 648($sp)
	lw $t2, 652($sp)
	mul $t0, $t1, $t2
	sw $t0, 656($sp)
	lw $t1, 644($sp)
	lw $t2, 656($sp)
	add $t0, $t1, $t2
	sw $t0, 660($sp)
	lw $t1, 660($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 664($sp)
	lw $t1, 124($sp)
	sw $t1, 668($sp)
	li $t1, 2
	sw $t1, 672($sp)
	lw $t1, 248($sp)
	sw $t1, 676($sp)
	lw $t1, 672($sp)
	lw $t2, 676($sp)
	mul $t0, $t1, $t2
	sw $t0, 680($sp)
	lw $t1, 668($sp)
	lw $t2, 680($sp)
	add $t0, $t1, $t2
	sw $t0, 684($sp)
	lw $t1, 684($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 688($sp)
	li $t1, 10
	sw $t1, 692($sp)
	lw $t1, 692($sp)
	sw $t1, -4($sp)
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	addi $sp, $sp, -316
	jal do_work
	addi $sp, $sp, 316
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	sw $v0, 696($sp)
	li $t1, 10
	sw $t1, 700($sp)
	lw $t1, 700($sp)
	sw $t1, -4($sp)
	lw $t1, 696($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 704($sp)
	lw $t1, 124($sp)
	sw $t1, 708($sp)
	lw $t1, 708($sp)
	sw $t1, -4($sp)
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	addi $sp, $sp, -316
	jal do_work
	addi $sp, $sp, 316
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	sw $v0, 712($sp)
	li $t1, 10
	sw $t1, 716($sp)
	lw $t1, 716($sp)
	sw $t1, -4($sp)
	lw $t1, 712($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 720($sp)
	lw $t1, 704($sp)
	lw $t2, 720($sp)
	beq $t1, $t2, label9
	j label10
label9:
	lw $t1, 124($sp)
	sw $t1, 724($sp)
	li $t1, 10
	sw $t1, 728($sp)
	lw $t1, 728($sp)
	sw $t1, -4($sp)
	lw $t1, 724($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 732($sp)
	lw $t1, 732($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 736($sp)
	j label11
label10:
	lw $t1, 124($sp)
	sw $t1, 740($sp)
	li $t1, 20
	sw $t1, 744($sp)
	lw $t1, 744($sp)
	sw $t1, -4($sp)
	lw $t1, 740($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 748($sp)
	lw $t1, 748($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 752($sp)
label11:
	lw $t1, 248($sp)
	sw $t1, 756($sp)
	lw $t1, 272($sp)
	sw $t1, 760($sp)
	lw $t1, 760($sp)
	sw $t1, -4($sp)
	lw $t1, 756($sp)
	sw $t1, -8($sp)
	addi $sp, $sp, -12
	sw $ra, 0($sp)
	addi $sp, $sp, -28
	jal mod
	addi $sp, $sp, 28
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	sw $v0, 764($sp)
	lw $t1, 764($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 768($sp)
	addi $t1, $sp, 276
	lw $t2, 768($sp)
	add $t0, $t1, $t2
	sw $t0, 772($sp)
	lw $t1, 124($sp)
	sw $t1, 776($sp)
	lw $t1, 124($sp)
	sw $t1, 780($sp)
	lw $t1, 776($sp)
	lw $t2, 780($sp)
	mul $t0, $t1, $t2
	sw $t0, 784($sp)
	lw $t1, 784($sp)
	lw $t2, 772($sp)
	sw $t1, 0($t2)
	lw $t1, 772($sp)
	lw $t1, 0($t1)
	sw $t1, 792($sp)
	lw $t1, 248($sp)
	sw $t1, 796($sp)
	li $t1, 1
	sw $t1, 800($sp)
	lw $t1, 796($sp)
	lw $t2, 800($sp)
	add $t0, $t1, $t2
	sw $t0, 804($sp)
	lw $t1, 804($sp)
	sw $t1, 248($sp)
	lw $t1, 248($sp)
	sw $t1, 808($sp)
	j label6
label8:
	lw $t1, 124($sp)
	sw $t1, 812($sp)
	li $t1, 0
	sw $t1, 816($sp)
	lw $t1, 816($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 820($sp)
	addi $t1, $sp, 276
	lw $t2, 820($sp)
	add $t0, $t1, $t2
	sw $t0, 824($sp)
	lw $t1, 824($sp)
	lw $t1, 0($t1)
	sw $t1, 828($sp)
	lw $t1, 812($sp)
	lw $t2, 828($sp)
	add $t0, $t1, $t2
	sw $t0, 832($sp)
	li $t1, 1
	sw $t1, 836($sp)
	lw $t1, 836($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 840($sp)
	addi $t1, $sp, 276
	lw $t2, 840($sp)
	add $t0, $t1, $t2
	sw $t0, 844($sp)
	lw $t1, 844($sp)
	lw $t1, 0($t1)
	sw $t1, 848($sp)
	lw $t1, 832($sp)
	lw $t2, 848($sp)
	add $t0, $t1, $t2
	sw $t0, 852($sp)
	lw $t1, 852($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 856($sp)
	lw $t1, 124($sp)
	sw $t1, 860($sp)
	li $t1, 0
	sw $t1, 864($sp)
	lw $t1, 864($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 868($sp)
	addi $t1, $sp, 276
	lw $t2, 868($sp)
	add $t0, $t1, $t2
	sw $t0, 872($sp)
	lw $t1, 872($sp)
	lw $t1, 0($t1)
	sw $t1, 876($sp)
	lw $t1, 860($sp)
	lw $t2, 876($sp)
	add $t0, $t1, $t2
	sw $t0, 880($sp)
	li $t1, 1
	sw $t1, 884($sp)
	lw $t1, 884($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 888($sp)
	addi $t1, $sp, 276
	lw $t2, 888($sp)
	add $t0, $t1, $t2
	sw $t0, 892($sp)
	lw $t1, 892($sp)
	lw $t1, 0($t1)
	sw $t1, 896($sp)
	lw $t1, 880($sp)
	lw $t2, 896($sp)
	add $t0, $t1, $t2
	sw $t0, 900($sp)
	lw $t1, 900($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 904($sp)
	lw $t1, 124($sp)
	sw $t1, 908($sp)
	li $t1, 0
	sw $t1, 912($sp)
	lw $t1, 912($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 916($sp)
	addi $t1, $sp, 276
	lw $t2, 916($sp)
	add $t0, $t1, $t2
	sw $t0, 920($sp)
	lw $t1, 920($sp)
	lw $t1, 0($t1)
	sw $t1, 924($sp)
	lw $t1, 908($sp)
	lw $t2, 924($sp)
	add $t0, $t1, $t2
	sw $t0, 928($sp)
	li $t1, 1
	sw $t1, 932($sp)
	lw $t1, 932($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 936($sp)
	addi $t1, $sp, 276
	lw $t2, 936($sp)
	add $t0, $t1, $t2
	sw $t0, 940($sp)
	lw $t1, 940($sp)
	lw $t1, 0($t1)
	sw $t1, 944($sp)
	lw $t1, 928($sp)
	lw $t2, 944($sp)
	add $t0, $t1, $t2
	sw $t0, 948($sp)
	lw $t1, 948($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 952($sp)
	lw $t1, 124($sp)
	sw $t1, 956($sp)
	li $t1, 0
	sw $t1, 960($sp)
	lw $t1, 960($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 964($sp)
	addi $t1, $sp, 276
	lw $t2, 964($sp)
	add $t0, $t1, $t2
	sw $t0, 968($sp)
	lw $t1, 968($sp)
	lw $t1, 0($t1)
	sw $t1, 972($sp)
	lw $t1, 956($sp)
	lw $t2, 972($sp)
	add $t0, $t1, $t2
	sw $t0, 976($sp)
	li $t1, 1
	sw $t1, 980($sp)
	lw $t1, 980($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 984($sp)
	addi $t1, $sp, 276
	lw $t2, 984($sp)
	add $t0, $t1, $t2
	sw $t0, 988($sp)
	lw $t1, 988($sp)
	lw $t1, 0($t1)
	sw $t1, 992($sp)
	lw $t1, 976($sp)
	lw $t2, 992($sp)
	add $t0, $t1, $t2
	sw $t0, 996($sp)
	lw $t1, 996($sp)
	sw $t1, 124($sp)
	lw $t1, 124($sp)
	sw $t1, 1000($sp)
	lw $t1, 124($sp)
	sw $t1, 1004($sp)
	lw $a0, 1004($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1008($sp)
	li $t1, 0
	sw $t1, 1012($sp)
	lw $t1, 1012($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 1016($sp)
	addi $t1, $sp, 276
	lw $t2, 1016($sp)
	add $t0, $t1, $t2
	sw $t0, 1020($sp)
	lw $t1, 1020($sp)
	lw $t1, 0($t1)
	sw $t1, 1024($sp)
	lw $a0, 1024($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1028($sp)
	li $t1, 1
	sw $t1, 1032($sp)
	lw $t1, 1032($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 1036($sp)
	addi $t1, $sp, 276
	lw $t2, 1036($sp)
	add $t0, $t1, $t2
	sw $t0, 1040($sp)
	lw $t1, 1040($sp)
	lw $t1, 0($t1)
	sw $t1, 1044($sp)
	lw $a0, 1044($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1048($sp)
	li $t1, 2
	sw $t1, 1052($sp)
	lw $t1, 1052($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 1056($sp)
	addi $t1, $sp, 276
	lw $t2, 1056($sp)
	add $t0, $t1, $t2
	sw $t0, 1060($sp)
	lw $t1, 1060($sp)
	lw $t1, 0($t1)
	sw $t1, 1064($sp)
	lw $a0, 1064($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1068($sp)
	li $t1, 3
	sw $t1, 1072($sp)
	lw $t1, 1072($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 1076($sp)
	addi $t1, $sp, 276
	lw $t2, 1076($sp)
	add $t0, $t1, $t2
	sw $t0, 1080($sp)
	lw $t1, 1080($sp)
	lw $t1, 0($t1)
	sw $t1, 1084($sp)
	lw $a0, 1084($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1088($sp)
	li $t1, 4
	sw $t1, 1092($sp)
	lw $t1, 1092($sp)
	li $t2, 4
	mul $t0, $t1, $t2
	sw $t0, 1096($sp)
	addi $t1, $sp, 276
	lw $t2, 1096($sp)
	add $t0, $t1, $t2
	sw $t0, 1100($sp)
	lw $t1, 1100($sp)
	lw $t1, 0($t1)
	sw $t1, 1104($sp)
	lw $a0, 1104($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t1, 0
	sw $t1, 1108($sp)
	li $t1, 0
	sw $t1, 1112($sp)
	lw $v0, 1112($sp)
	jr $ra
	addi $sp, $sp, 1116

