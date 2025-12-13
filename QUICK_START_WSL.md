# دليل البدء السريع - WSL

## الخطوة 1: إعداد MySQL على Windows

MySQL على Windows يحتاج إلى السماح بالاتصالات من WSL. قم بتشغيل هذا الأمر **على Windows** (في PowerShell أو CMD):

```powershell
# في PowerShell على Windows
cd C:\Users\yasee\.cursor\worktrees\UniversityApp\net
mysql -u root -pyaseentarek < setup_mysql_windows.sql
```

أو يدوياً في MySQL:

```sql
CREATE USER 'root'@'%' IDENTIFIED BY 'yaseentarek';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

**ملاحظة**: قد تحتاج أيضاً إلى تعديل `my.ini` في MySQL:
```
bind-address = 0.0.0.0
```

## الخطوة 2: إعداد قاعدة البيانات من WSL

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
bash setup_database.sh
```

## الخطوة 3: تثبيت Qt و CMake

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev qt6-base-dev-tools libqt6sql6-mysql
```

## الخطوة 4: بناء المشروع

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
mkdir -p build
cd build
cmake ..
cmake --build . --config Release
```

## الخطوة 5: تشغيل التطبيق

```bash
./NCTU_SIS
```

عند فتح التطبيق:
- **Host**: استخدم IP Windows (عادة `10.255.255.254`) أو `localhost`
- **User**: `root`
- **Password**: `yaseentarek`
- **Database**: `nctu_sis`

## استكشاف الأخطاء

### لا يمكن الاتصال بـ MySQL

1. تأكد من تشغيل MySQL على Windows:
   ```powershell
   Get-Service MySQL*
   ```

2. جرب الاتصال من WSL:
   ```bash
   mysql -h 10.255.255.254 -u root -pyaseentarek -e "SELECT 1;"
   ```

3. تحقق من إعدادات MySQL في Windows

### QMYSQL driver not loaded

```bash
sudo apt-get install -y libqt6sql6-mysql
```

### CMake not found

```bash
sudo apt-get install -y cmake
```

