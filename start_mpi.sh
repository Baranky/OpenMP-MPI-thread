#!/bin/bash

# Düğümleri kontrol et
NODES=""
for i in {1..4}; do
    if docker ps | grep -q "node$i"; then
        NODES="$NODES,node$i"
    fi
done

# İlk virgülü kaldır
NODES=${NODES#,}

# Çalışan düğüm sayısını hesapla
NODE_COUNT=$(echo $NODES | tr ',' '\n' | wc -l)

if [ $NODE_COUNT -eq 0 ]; then
    echo "Hata: Hiç çalışan düğüm bulunamadı!"
    exit 1
fi

echo "Çalışan düğümler: $NODES"
echo "Düğüm sayısı: $NODE_COUNT"

# MPI programını çalıştır
mpirun -n $NODE_COUNT -host $NODES -bind-to none -map-by slot ./parallel_processing 