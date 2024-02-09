![Reverse_ROP Logo](https://github.com/Loqiu971/Reverse_ROP/blob/main/icone.png "Reverse_ROP logo")

# Reverse ROP [PROG: PYTHON]

#Sujet:

L’objectif de cette suite d’exercice est de découvrir les techniques basiques puis avancées
d’attaque logiciel et notamment le ROP (Return Oriented Programming) dans le cadre d’un
système avec pile non exécutable. Vous allez devoir compiler les différents niveaux d’un
programme défaillant via le Makefile.

```bash
# pour compiler le niveau 1
user$ LEVEL=1 make clean all
# pour compiler TOUS les niveaux
user$ make all_levels
``` 

Pour chaque niveau, un programme my_vmN est généré. Il s’agit d’une mini machine
virtuelle capable d’interpréter des instructions basiques. Comprendre ce que fait la machine
est nécessaire car il s’agit de trouver ses failles et de les exploiter. La machine compile avec
le “SetUserId Bit”, c’est à dire que le programme s’exécutera avec les droits du propriétaire
du fichier. Pour mieux saisir, l’impact de l’exploitation d’un tel programme le Makefile
changera le propriétaire du programme produit par root, et activera le SetUserId Bit. Pour ce
faire le Makefile effectura un sudo, et vous demandera de saisir votre mot de passe et VULN 
nécessite que vous soyez dans le groupe de sudoers. Nous vous demandons d’utiliser
“Pwntools” et de nous rendre des scripts pythons fabriquant les payloads malveillantes
nécessaire et lorsque nécessaire automatisant l’attaque afin d’obtenir un shell ROOT sur la
machine.

Pour le rattrapage vous devez atteindre le niveau 4.
Contexte¶
La société CorpNet a développé un prototype d’interface Neuro-biologique Turing Complet
interprétant dans le cerveau d’une grenouille des instructions d’une machine “Biologique”. A
cette fin et pour des raisons obscures le programme d’exécution des tests de ces instructions
machines s’exécutent avec les droits root via les droits “SetUserId Bit”. Normalement, le
programme cherche à accéder à un périphérique obscure “/dev/frog”. Ce périphérique étant
directement connecté au cerveau de la grenouille. Vous avez obtenue le code source d’une
version de test du programme finale.


3. CLI: Command Line Interface:¶

L’exécution du programme my_vmX lorsqu’aucun programme n’est passé en paramètre
passe en mode CLI, et lit les commandes sur l’entrée standard. Vous pouvez directement
saisir des commandes du processeur grenouille, ainsi que 2 commandes spécifiques de
l’interface CLI:

EXEC : Exécute toutes les commandes précédemment tapées
INCLUDE File.Insn : Charge et exécute les commandes à partir du fichier “File.insn”

```bash
user$ ./my_vm1
Simple Virtual Machine helping you to understand BOF/ROP/etc...
Enter command: load_b 12
Enter command: load_c 4
Enter command: add
Enter command: dump
Enter command: exec
A:16 B:12 C:4 SI:0 DI:0 IP:3
Enter command:
```

3. 1. Niveaux¶
Niveau 1¶
Ce niveau est un niveau de prise en main. Le programme est seulement compilé avec une pile
non exécutable... Vous devez comprendre le code de la Vm qui vous est fourni car il contient
une faille qu’il va falloir exploiter. Il s’agit donc de pratiquer un buffer overflow de tel
manière qu’une certaine fonction de vuln.c soit appelé. Il va donc falloir lire le code source et
le comprendre ! Pour vous aider, répondez au formulaire SATA: my_vm Votre payload peut
être généré à froid une bonne fois pour toute car l’ASLR est désactivé, donc les adresses sont
fixes.
Votre script ‘./exploit1.py’ génère un fichier xploit1.insn qu’on passe directement à
l’interpréteur.

```bash
user$ ./exploit1.py
user$ ./my_vm1 ./xploit1.insn
$ whoami
root
```

Niveau 2¶
Similaire au niveau 1, il faut toutefois passer un argument à la fonction vers le bon paramètre
pour invoquer execve avec /bin/sh. Regarder les astuces de Shellcode du précis.

Niveau 3¶
Similaire au niveau 2 mais avec un canary en plus et sans ASLR. Vous allez devoir via une
technique interactive automatisé obtenir des fuites d’informations pour deviner le canary. A
partir de ce niveau, votre exploit peut foirer de temps en temps. Pas de panique! Vous ne
serez pas pénalisé sur ce point.
```bash
user$ ./exploit3.py
$ whoami
root
```

Niveau 4¶
L’ASLR est actif et un canary va vous barrer le chemin. Vous allez devoir deviner en plus du
canary les adresses relatives au run du programme afin de forger une payload à chaud vous
permettant d’exécuter pwn_func4 avec le bon paramètre.