import numpy as np

# 10 milyon sayı oluştur
data = np.random.rand(10000000)

# Veriyi dosyaya kaydet
np.save('data/numbers.npy', data)

print("Veri dosyası oluşturuldu: data/numbers.npy")
print(f"Veri boyutu: {len(data)} sayı")
print(f"Veri aralığı: [{data.min():.4f}, {data.max():.4f}]")
print(f"Ortalama: {data.mean():.4f}")
