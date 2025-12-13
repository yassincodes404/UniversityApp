# إعداد WSL لتشغيل NCTU SIS

## الخطوات اليدوية

### 1. تثبيت MySQL Client في WSL

```bash
sudo apt-get update
sudo apt-get install -y mysql-client
```

### 2. اختبار الاتصال بـ MySQL على Windows

من WSL، MySQL على Windows يمكن الوصول إليه عبر:
- **Windows Host IP**: عادة `10.255.255.254` (من `/etc/resolv.conf`)
- أو **localhost**: في بعض الحالات

```bash
# اختبار الاتصال
mysql -h 10.255.255.254 -u root -pyaseentarek -e "SELECT VERSION();"

# أو
mysql -h localhost -u root -pyaseentarek -e "SELECT VERSION();"
```

### 3. إعداد قاعدة البيانات

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net

# إنشاء قاعدة البيانات
mysql -h 10.255.255.254 -u root -pyaseentarek < db/mysql_create.sql

# إدخال البيانات التجريبية
mysql -h 10.255.255.254 -u root -pyaseentarek < db/seed_data.sql
```

### 4. تثبيت Qt و CMake

```bash
sudo apt-get install -y build-essential cmake qt6-base-dev qt6-base-dev-tools libqt6sql6-mysql
```

### 5. بناء المشروع

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
mkdir -p build
cd build
cmake ..
cmake --build . --config Release
```

### 6. تشغيل التطبيق

```bash
./NCTU_SIS
```

## ملاحظات مهمة

1. **MySQL على Windows**: تأكد من أن MySQL يعمل على Windows ويمكن الوصول إليه من WSL
2. **Firewall**: قد تحتاج لإضافة قاعدة في Windows Firewall للسماح بالاتصالات من WSL
3. **MySQL Remote Access**: تأكد من أن MySQL يسمح بالاتصالات من خارج localhost:
   ```sql
   -- في MySQL على Windows
   CREATE USER 'root'@'%' IDENTIFIED BY 'yaseentarek';
   GRANT ALL PRIVILEGES ON *.* TO 'root'@'%';
   FLUSH PRIVILEGES;
   ```

## استكشاف الأخطاء

### خطأ: "Can't connect to MySQL server"

1. تحقق من أن MySQL يعمل على Windows:
   ```powershell
   # في PowerShell على Windows
   Get-Service MySQL*
   ```

2. تحقق من إعدادات MySQL:
   - افتح `my.ini` أو `my.cnf`
   - تأكد من `bind-address = 0.0.0.0` (وليس `127.0.0.1`)

3. جرب الاتصال من Windows أولاً:
   ```cmd
   mysql -u root -pyaseentarek -h localhost
   ```

### خطأ: "QMYSQL driver not loaded"

```bash
sudo apt-get install -y libqt6sql6-mysql
```

### خطأ: "CMake not found"

```bash
sudo apt-get install -y cmake
```

