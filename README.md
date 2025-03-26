# Dağıtık Sistemler Ödevi - MPI ve OpenMP ile Paralel İşleme

Bu proje, Docker kullanarak birden fazla düğümlü bir dağıtık sistem oluşturur ve MPI ile OpenMP'yi birlikte kullanarak paralel veri işleme gerçekleştirir.

## Gereksinimler

- Docker
- Docker Compose

## Proje Yapısı

- `Dockerfile`: MPI ve OpenMP ortamını içeren Docker imajı
- `docker-compose.yml`: Birden fazla düğümü yapılandıran Docker Compose dosyası
- `parallel_processing.c`: MPI ve OpenMP kullanan ana program
- `data/`: Veri dosyaları için dizin

## Kurulum ve Çalıştırma

1. Projeyi klonlayın:
```bash
git clone <repo-url>
cd <proje-dizini>
```

2. Docker imajını oluşturun ve konteynerleri başlatın:
```bash
docker-compose up --build
```

## Nasıl Çalışır?

1. Sistem 4 düğümden oluşur (node1, node2, node3, node4)
2. Her düğüm 2 OpenMP thread'i kullanır
3. MPI ile düğümler arası iletişim sağlanır
4. Veri düğümler arasında dağıtılır ve paralel olarak işlenir
5. Sonuçlar ana düğümde (node1) toplanır

## Özelleştirme

- `DATA_SIZE`: İşlenecek veri boyutu
- `CHUNK_SIZE`: Her işlem için veri parçası boyutu
- `OMP_NUM_THREADS`: Her düğümdeki OpenMP thread sayısı
- Düğüm sayısı: docker-compose.yml dosyasında değiştirilebilir 