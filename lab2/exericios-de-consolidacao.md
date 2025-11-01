### 1. Descreva o funcionamento da função fork(). Após o fork, como os processos pai e filho podem se comunicar/sincronizar (considere apenas as chamadas fork(), exec(), exit() e wait())?

Fork é uma SVC utilizada para criar um novo processo a partir de um atual. Todos os processos no Sistema Operacional são criados a partir do init. O fork é dividio em alguns passos:

* Alocar um novo PCB para o novo processo. Esse PCB é identico ao do pai tirando alguns campos como PID e PPID.
    * O filho herda vários atributos do pai, como quantos e quais arquivos estão abertos por ele.
* Duplicar a imagem do processo. No caso do Copy-on-Write, a cópia só acontece se o pai ou filho tentar modificar alguma página das estruturas da imagem (como pilha, heap, data section etc)
* Para o pai, o `fork();` retorna o PID do filho criado ou -1 em caso de falha.
* Quando o filho ganhar a posse da CPU pela primeira vez, seu estado será de Kernel Running para finalizar a chamada do `fork();`
* Para o filho, o `fork();` retorna zero. Assim conseguimos diferenciar o fluxo de execução de múltiplos processos num mesmo código.

O Copy-on-Write é importante pois muita das vezes cria-se um filho e imediatamente depois substitui toda sua imagem. Então seria um esforço computacional desncessário copiar a imagem inteira. A filosofia é a seguinte: enquanto pai e filhos conseguirem compartilhar os mesmos espaços de memória, não os separe. O ponto de separação acontece justamente quando um tenta fazer uma escrita. Nesse caso, o Sistema Operacional bloqueia ambos e faz a separação da página (cada processo terá agora uma moldura diferente).

Exit é uma SVC utilizada para finalizar um processo. Exit:

* chama todos os *exit handlers*.
* libera a memória do segmento de dados
* fecha os arquivos abertos pelo processo
* envia o sinal SIGHLD para o processo pai
* caso o processo tenha filhos, estes são adotados pelo **init** ou pelo **systemd**
* invoca o escalonador

Wait é uma SVC para o processo pai esperar **término de processos**. É possível também enviar um `int *` que terá uma série de flags que indicará o status do processo. De acordo com a necessidade, esse `int *` deve manipulado pelas macros especificadas pelo POSIX, como WIFEXITED e WEXITSTATUS. Ao chamar `wait(int *);`, o processo chamador entra em suspensão até que **algum** de seus filhos termine.

Exec é uma SVC para substituição da imagem do processo, matendo apenas coisas cruciais como PID e PPID. A partir da exec, um processo passará a executar outro código fonte se realizado com êxito.
