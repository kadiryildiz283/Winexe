# Makefile
# Derleyici seçimi
CC = g++
# Derleme seçenekleri
CFLAGS = -Wall -std=c++17
# Kaynak dosyalarının bulunduğu dizin
SRCDIR = src
# Başlık dosyalarının bulunduğu dizin
INCDIR = include
# Çıktı dosyalarının bulunduğu dizin
BINDIR = bin
# Kaynak dosyalarının listesi
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Nesne dosyalarının listesi
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))
# Çalıştırılabilir dosyanın adı
EXECUTABLE = main
# Varsayılan hedef
all: $(EXECUTABLE)
# Çalıştırılabilir dosyayı oluşturmak için bağımlılık ve komut
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
# .cpp dosyalarını .o dosyalarına dönüştürmek için bağımlılık ve komut
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

# Oluşturulan dosyaları silmek için temizleme komutu
clean:
	rm -f $(EXECUTABLE) && rm -f $(OBJECTS)

run:
	@./main
