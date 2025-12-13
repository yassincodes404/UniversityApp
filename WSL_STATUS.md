# حالة الإعداد - WSL

## ✅ ما تم إنجازه

1. ✅ **MySQL Client مثبت** في WSL
2. ✅ **سكربتات الإعداد جاهزة**:
   - `setup_database.sh` - لإعداد قاعدة البيانات
   - `setup_mysql_windows.sql` - لتكوين MySQL على Windows
3. ✅ **الكود محدث** - `main.cpp` يكتشف تلقائياً IP Windows

## ⚠️ ما يحتاج إلى إعداد

### 1. إعداد MySQL على Windows (مطلوب)

MySQL على Windows يحتاج إلى السماح بالاتصالات من WSL. قم بتنفيذ أحد الخيارات التالية:

#### الخيار 1: استخدام السكربت (الأسهل)

**على Windows** (في PowerShell أو CMD):
```powershell
cd C:\Users\yasee\.cursor\worktrees\UniversityApp\net
mysql -u root -pyaseentarek < setup_mysql_windows.sql
```

#### الخيار 2: يدوياً في MySQL

```sql
CREATE USER 'root'@'%' IDENTIFIED BY 'yaseentarek';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

#### الخيار 3: تعديل my.ini

افتح `my.ini` في مجلد MySQL (عادة `C:\ProgramData\MySQL\MySQL Server 8.0\my.ini`) وأضف/عدّل:

```ini
bind-address = 0.0.0.0
```

ثم أعد تشغيل MySQL service.

### 2. تثبيت Qt و CMake (إذا لم تكن مثبتة)

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev qt6-base-dev-tools libqt6sql6-mysql
```

### 3. إعداد قاعدة البيانات

بعد إعداد MySQL على Windows:

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
bash setup_database.sh
```

### 4. بناء المشروع

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
mkdir -p build
cd build
cmake ..
cmake --build . --config Release
```

### 5. تشغيل التطبيق

```bash
./NCTU_SIS
```

## اختبار الاتصال

بعد إعداد MySQL على Windows، اختبر الاتصال من WSL:

```bash
# احصل على Windows host IP
WINDOWS_IP=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')

# اختبر الاتصال
mysql -h $WINDOWS_IP -u root -pyaseentarek -e "SELECT VERSION();"
```

إذا نجح، ستظهر نسخة MySQL.

## الخطوات التالية

1. **قم بإعداد MySQL على Windows** (الخطوة 1 أعلاه)
2. **اختبر الاتصال** من WSL
3. **شغّل `setup_database.sh`** لإعداد قاعدة البيانات
4. **ثبت Qt و CMake** إذا لزم الأمر
5. **ابنِ وشغّل المشروع**

## المساعدة

إذا واجهت مشاكل:
- راجع `QUICK_START_WSL.md` للدليل الكامل
- راجع `WSL_SETUP.md` لتفاصيل إضافية
- تأكد من أن MySQL يعمل على Windows: `Get-Service MySQL*` في PowerShell

