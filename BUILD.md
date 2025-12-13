# دليل البناء - NCTU SIS

## المتطلبات الأساسية

### Windows

1. **Qt**: قم بتحميل Qt من [qt.io](https://www.qt.io/download)
   - اختر Qt 5.15+ أو Qt 6.x
   - تأكد من تثبيت Qt Creator
   - أضف Qt إلى PATH

2. **CMake**: قم بتحميل من [cmake.org](https://cmake.org/download/)
   - أو استخدم: `winget install Kitware.CMake`

3. **MySQL**: 
   - قم بتحميل MySQL Server من [mysql.com](https://dev.mysql.com/downloads/mysql/)
   - أو استخدم: `winget install Oracle.MySQL.Server`
   - تأكد من تثبيت MySQL Connector/C

4. **مترجم C++**:
   - Visual Studio 2019+ (مع C++ workload)
   - أو MinGW-w64

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-base-dev-tools \
                 libqt6sql6-mysql mysql-server mysql-client
```

### macOS

```bash
brew install qt@6 cmake mysql
```

## خطوات البناء

### 1. استنساخ المشروع

```bash
git clone <repository-url>
cd NCTU_SIS
```

### 2. إعداد قاعدة البيانات

راجع `db/README.md` لإعداد قاعدة البيانات.

### 3. البناء باستخدام CMake

```bash
# إنشاء مجلد البناء
mkdir build
cd build

# تكوين المشروع
cmake .. -DCMAKE_BUILD_TYPE=Release

# البناء
cmake --build . --config Release
```

### 4. البناء باستخدام Qt Creator

1. افتح Qt Creator
2. اختر `File > Open File or Project`
3. اختر `CMakeLists.txt`
4. اختر Kit (مترجم Qt)
5. اضغط `Configure Project`
6. اضغط `Build` (Ctrl+B)

## حل المشاكل الشائعة

### خطأ: "QMYSQL driver not loaded"

**Windows**:
- تأكد من وجود `qsqlmysql.dll` في `Qt/plugins/sqldrivers/`
- إذا لم يكن موجوداً، قم ببناءه من مصادر Qt

**Linux**:
```bash
sudo apt install libqt6sql6-mysql
```

**macOS**:
```bash
brew install mysql
# ثم أعد بناء Qt مع دعم MySQL
```

### خطأ: "Cannot find Qt"

أضف Qt إلى PATH:
```bash
# Windows (PowerShell)
$env:PATH += ";C:\Qt\6.x.x\msvc2019_64\bin"

# Linux/macOS
export PATH=$PATH:/path/to/qt/bin
```

### خطأ: "MySQL connection failed"

1. تأكد من تشغيل MySQL:
```bash
# Windows
net start MySQL80

# Linux
sudo systemctl start mysql

# macOS
brew services start mysql
```

2. تحقق من بيانات الاتصال في `main.cpp` أو في نافذة الإعدادات

## البناء للتوزيع

### Windows

```bash
# استخدام windeployqt
windeployqt.exe NCTU_SIS.exe

# أو استخدام NSIS/Inno Setup لإنشاء مثبت
```

### Linux

```bash
# استخدام linuxdeployqt أو AppImage
```

### macOS

```bash
# استخدام macdeployqt
macdeployqt NCTU_SIS.app
```

## الاختبار

بعد البناء، اختبر التطبيق:

1. شغّل التطبيق
2. أدخل بيانات قاعدة البيانات
3. جرب تسجيل الدخول باستخدام البيانات التجريبية
4. اختبر الوظائف الأساسية

## الأداء

- استخدم `Release` build للإنتاج
- قم بتحسين استعلامات قاعدة البيانات
- استخدم الفهارس بشكل صحيح
- راقب استخدام الذاكرة

## الدعم

إذا واجهت مشاكل في البناء:
1. تحقق من المتطلبات
2. راجع ملفات السجل
3. تأكد من إصدارات الأدوات
4. افتح issue في المستودع

