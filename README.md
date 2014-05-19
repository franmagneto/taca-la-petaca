Taca la Petaca - Módulo de kernel
=================================

Autor: Francisco Machado Magalhães Neto

Maio de 2014

Módulo para o kernel do Linux que cria o dispositivo `/dev/taca`, que
contém o vídeo *Chapolin - Taca La Petaca*, e a entrada `/proc/taca`,
com a letra da música.

O código foi baseado no álbum *Cycles Per Instruction*, da banda
[netcat](http://www.netcat.co/), lançado como módulo de kernel.

Também foi usado como referência o livro *Linux Device Drivers*.

Uso
---

Para reproduzir o vídeo:

```
ffplay /dev/taca
```

Para ver a letra:

```
cat /proc/taca
```

Referências
-----------

* Linux Device Drivers, Third Edition
(<http://lwn.net/Kernel/LDD3/>)
* netcat - Cycles Per Instruction
(<http://github.com/usrbinnc/netcat-cpi-kernel-module>)
* Chapolin - Taca La Petaca - YouTube
(<http://www.youtube.com/watch?v=Nq-6IaeDP48>)
* TACA LA PETACA - Chaves | Letras.mus.br
(<http://letras.mus.br/chaves/1095878/>)
