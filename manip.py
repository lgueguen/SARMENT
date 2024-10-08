import partition
import sequence
import matrice
import lexique
import math

lx=lexique.Lexique(fprop="prob")

m=matrice.Matrice()
s=sequence.Sequence(fic="lambda.seq")
m.prediction(s,lx)

lx2=lexique.Lexique(str="1:#1 2:#2 3:#3 4:#4")
lx2.init_trans()

lx2.g_trans(1,1,math.log(0.9985))
lx2.g_trans(1,2,math.log(0.0005))
lx2.g_trans(1,3,math.log(0.0005))
lx2.g_trans(1,4,math.log(0.0005))
lx2.g_trans(2,2,math.log(0.9985))
lx2.g_trans(2,1,math.log(0.0005))
lx2.g_trans(2,3,math.log(0.0005))
lx2.g_trans(2,4,math.log(0.0005))
lx2.g_trans(3,3,math.log(0.9985))
lx2.g_trans(3,2,math.log(0.0005))
lx2.g_trans(3,1,math.log(0.0005))
lx2.g_trans(3,4,math.log(0.0005))
lx2.g_trans(4,4,math.log(0.9985))
lx2.g_trans(4,2,math.log(0.0005))
lx2.g_trans(4,3,math.log(0.0005))
lx2.g_trans(4,1,math.log(0.0005))


p_vit=partition.Partition()
p_vit.viterbi(m,lx2)

m_fb=matrice.Matrice()
m_fb.fb(m,lx2)
p_fb=partition.Partition()
p_fb.read_Matrice(m_fb)
