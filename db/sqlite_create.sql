-- SQLite Database Schema for NCTU SIS
-- Converted from MySQL to SQLite

-- Enable foreign keys
PRAGMA foreign_keys = ON;

-- Departments
CREATE TABLE IF NOT EXISTS departments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  description TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Programs
CREATE TABLE IF NOT EXISTS programs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  department_id INTEGER,
  years INTEGER DEFAULT 4,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Students
CREATE TABLE IF NOT EXISTS students (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_code TEXT UNIQUE NOT NULL,
  name TEXT NOT NULL,
  email TEXT UNIQUE,
  phone TEXT,
  password_hash TEXT NOT NULL,
  salt TEXT NOT NULL,
  program_id INTEGER,
  department_id INTEGER,
  national_id TEXT,
  birth_date DATE,
  gender TEXT CHECK(gender IN ('M','F','Other')) DEFAULT 'Other',
  seat_number TEXT UNIQUE,
  status TEXT CHECK(status IN ('active','graduated','suspended')) DEFAULT 'active',
  level INTEGER DEFAULT 1,
  credits_completed INTEGER DEFAULT 0,
  gpa REAL DEFAULT 0.00,
  must_change_password INTEGER DEFAULT 0,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Doctors (instructors)
CREATE TABLE IF NOT EXISTS doctors (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  email TEXT UNIQUE,
  phone TEXT,
  password_hash TEXT NOT NULL,
  salt TEXT NOT NULL,
  department_id INTEGER,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Labs
CREATE TABLE IF NOT EXISTS labs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  lab_name TEXT NOT NULL,
  capacity INTEGER DEFAULT 0,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Courses
CREATE TABLE IF NOT EXISTS courses (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT NOT NULL,
  name TEXT NOT NULL,
  credit_hours INTEGER DEFAULT 3,
  program_id INTEGER,
  year INTEGER,
  semester TEXT CHECK(semester IN ('Fall','Spring')),
  course_type TEXT CHECK(course_type IN ('Theory','Practical','Mixed')) DEFAULT 'Theory',
  description TEXT,
  prerequisites TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  UNIQUE (code, program_id)
);

-- Sections (lecture/lab/section)
CREATE TABLE IF NOT EXISTS sections (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  section_type TEXT CHECK(section_type IN ('Lecture','Section','Lab')) NOT NULL,
  course_id INTEGER,
  doctor_id INTEGER,
  lab_id INTEGER,
  day TEXT,
  time_slot TEXT,
  location TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (doctor_id) REFERENCES doctors(id) ON DELETE SET NULL,
  FOREIGN KEY (lab_id) REFERENCES labs(id) ON DELETE SET NULL
);

-- Student_Sections (many-to-many)
CREATE TABLE IF NOT EXISTS student_sections (
  student_id INTEGER,
  section_id INTEGER,
  enrolled_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (student_id, section_id),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
);

-- Enrollments (per semester)
CREATE TABLE IF NOT EXISTS enrollments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  course_id INTEGER NOT NULL,
  semester INTEGER NOT NULL,
  year INTEGER NOT NULL,
  status TEXT CHECK(status IN ('registered','dropped','pending')) DEFAULT 'pending',
  attempt_number INTEGER DEFAULT 1,
  registered_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (student_id, course_id, semester, year, attempt_number),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Grades (per course registration)
CREATE TABLE IF NOT EXISTS grades (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  enrollment_id INTEGER,
  student_id INTEGER NOT NULL,
  course_id INTEGER NOT NULL,
  midterm1 REAL,
  midterm2 REAL,
  final_exam REAL,
  practical REAL,
  assignments REAL,
  total REAL,
  letter_grade TEXT,
  result_status TEXT CHECK(result_status IN ('pass','fail','barred')) DEFAULT 'fail',
  is_published INTEGER DEFAULT 0,
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (enrollment_id) REFERENCES enrollments(id) ON DELETE CASCADE,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Attendance
CREATE TABLE IF NOT EXISTS attendance (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  section_id INTEGER NOT NULL,
  attendance_date DATE NOT NULL,
  status TEXT CHECK(status IN ('present','absent','late','excused')) NOT NULL,
  remark TEXT,
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (student_id, section_id, attendance_date),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
);

-- Admins
CREATE TABLE IF NOT EXISTS admins (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  email TEXT UNIQUE,
  password_hash TEXT NOT NULL,
  salt TEXT NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Permissions
CREATE TABLE IF NOT EXISTS permissions (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  admin_id INTEGER NOT NULL,
  operation TEXT NOT NULL,
  resource TEXT NOT NULL,
  granted_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (admin_id) REFERENCES admins(id) ON DELETE CASCADE
);

-- Payments
CREATE TABLE IF NOT EXISTS payments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  amount REAL,
  semester INTEGER,
  payment_date DATETIME,
  method TEXT CHECK(method IN ('Online','Cash')) DEFAULT 'Online',
  status TEXT CHECK(status IN ('Paid','Pending')) DEFAULT 'Pending',
  notes TEXT,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- News
CREATE TABLE IF NOT EXISTS news (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  title TEXT NOT NULL,
  content TEXT NOT NULL,
  news_date DATE,
  created_by INTEGER,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (created_by) REFERENCES admins(id) ON DELETE SET NULL
);

-- Exams
CREATE TABLE IF NOT EXISTS exams (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_id INTEGER NOT NULL,
  exam_type TEXT CHECK(exam_type IN ('Midterm','Final')) NOT NULL,
  exam_date DATE NOT NULL,
  year INTEGER NOT NULL,
  semester INTEGER NOT NULL,
  duration_minutes INTEGER,
  venue TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Exam Results
CREATE TABLE IF NOT EXISTS exam_results (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  exam_id INTEGER NOT NULL,
  student_id INTEGER NOT NULL,
  degree REAL,
  status TEXT CHECK(status IN ('pass','fail','pmf','barred')),
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (exam_id, student_id),
  FOREIGN KEY (exam_id) REFERENCES exams(id) ON DELETE CASCADE,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- PMD table
CREATE TABLE IF NOT EXISTS pmd_records (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  course_id INTEGER NOT NULL,
  exam_id INTEGER,
  pmd_status TEXT CHECK(pmd_status IN ('PASS','MEDICAL','DELAYED')) NOT NULL,
  notes TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (exam_id) REFERENCES exams(id) ON DELETE SET NULL
);

-- Audit log
CREATE TABLE IF NOT EXISTS audit_log (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  actor_type TEXT, -- 'admin','doctor'
  actor_id INTEGER,
  action TEXT,
  detail TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_students_code ON students(student_code);
CREATE INDEX IF NOT EXISTS idx_students_email ON students(email);
CREATE INDEX IF NOT EXISTS idx_enrollments_student ON enrollments(student_id);
CREATE INDEX IF NOT EXISTS idx_enrollments_course ON enrollments(course_id);
CREATE INDEX IF NOT EXISTS idx_grades_student ON grades(student_id);
CREATE INDEX IF NOT EXISTS idx_grades_course ON grades(course_id);
CREATE INDEX IF NOT EXISTS idx_attendance_student ON attendance(student_id);
CREATE INDEX IF NOT EXISTS idx_attendance_section ON attendance(section_id);
CREATE INDEX IF NOT EXISTS idx_sections_course ON sections(course_id);
CREATE INDEX IF NOT EXISTS idx_sections_doctor ON sections(doctor_id);

