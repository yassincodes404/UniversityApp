-- mysql_create.sql
-- NCTU SIS Database Schema
-- Run: mysql -u root -p < mysql_create.sql

CREATE DATABASE IF NOT EXISTS nctu_sis CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE nctu_sis;

-- Departments
CREATE TABLE departments (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  description TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Programs
CREATE TABLE programs (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  department_id INT,
  years INT DEFAULT 4,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Students
CREATE TABLE students (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_code VARCHAR(32) UNIQUE NOT NULL,
  name VARCHAR(255) NOT NULL,
  email VARCHAR(255) UNIQUE,
  phone VARCHAR(50),
  password_hash VARCHAR(255) NOT NULL,
  salt VARCHAR(64) NOT NULL,
  program_id INT,
  department_id INT,
  national_id VARCHAR(50),
  birth_date DATE,
  gender ENUM('M','F','Other') DEFAULT 'Other',
  seat_number VARCHAR(50) UNIQUE,
  status ENUM('active','graduated','suspended') DEFAULT 'active',
  level INT DEFAULT 1,
  credits_completed INT DEFAULT 0,
  gpa DECIMAL(3,2) DEFAULT 0.00,
  must_change_password BOOLEAN DEFAULT TRUE,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Doctors (instructors)
CREATE TABLE doctors (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  email VARCHAR(255) UNIQUE,
  phone VARCHAR(50),
  password_hash VARCHAR(255) NOT NULL,
  salt VARCHAR(64) NOT NULL,
  department_id INT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Labs
CREATE TABLE labs (
  id INT AUTO_INCREMENT PRIMARY KEY,
  lab_name VARCHAR(255) NOT NULL,
  capacity INT DEFAULT 0,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Courses
CREATE TABLE courses (
  id INT AUTO_INCREMENT PRIMARY KEY,
  code VARCHAR(32) NOT NULL,
  name VARCHAR(255) NOT NULL,
  credit_hours INT DEFAULT 3,
  program_id INT,
  year INT,
  semester ENUM('Fall','Spring'),
  course_type ENUM('Theory','Practical','Mixed') DEFAULT 'Theory',
  description TEXT,
  prerequisites TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  UNIQUE (code, program_id)
);

-- Sections (lecture/lab/section)
CREATE TABLE sections (
  id INT AUTO_INCREMENT PRIMARY KEY,
  section_type ENUM('Lecture','Section','Lab') NOT NULL,
  course_id INT,
  doctor_id INT,
  lab_id INT,
  day VARCHAR(32),
  time_slot VARCHAR(64),
  location VARCHAR(255),
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (doctor_id) REFERENCES doctors(id) ON DELETE SET NULL,
  FOREIGN KEY (lab_id) REFERENCES labs(id) ON DELETE SET NULL
);

-- Student_Sections (many-to-many)
CREATE TABLE student_sections (
  student_id INT,
  section_id INT,
  enrolled_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (student_id, section_id),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
);

-- Enrollments (per semester)
CREATE TABLE enrollments (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  course_id INT NOT NULL,
  semester INT NOT NULL,
  year INT NOT NULL,
  status ENUM('registered','dropped','pending') DEFAULT 'pending',
  attempt_number INT DEFAULT 1,
  registered_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (student_id, course_id, semester, year, attempt_number),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Grades (per course registration)
CREATE TABLE grades (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  course_id INT NOT NULL,
  enrollment_id INT,
  midterm1 DECIMAL(5,2),
  midterm2 DECIMAL(5,2),
  final_exam DECIMAL(5,2),
  practical DECIMAL(5,2),
  assignments DECIMAL(5,2),
  total DECIMAL(5,2),
  letter_grade VARCHAR(4),
  result_status ENUM('pass','fail','barred') DEFAULT 'fail',
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (student_id, course_id, enrollment_id),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (enrollment_id) REFERENCES enrollments(id) ON DELETE SET NULL
);

-- Attendance
CREATE TABLE attendance (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  section_id INT NOT NULL,
  attendance_date DATE NOT NULL,
  status ENUM('present','absent','late','excused') NOT NULL,
  remark TEXT,
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (student_id, section_id, attendance_date),
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE
);

-- Admins + permissions
CREATE TABLE admins (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  email VARCHAR(255) UNIQUE,
  password_hash VARCHAR(255) NOT NULL,
  salt VARCHAR(64) NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE permissions (
  id INT AUTO_INCREMENT PRIMARY KEY,
  admin_id INT NOT NULL,
  operation VARCHAR(64) NOT NULL,
  resource VARCHAR(64) NOT NULL,
  granted_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (admin_id) REFERENCES admins(id) ON DELETE CASCADE
);

-- Payments
CREATE TABLE payments (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  amount DECIMAL(10,2),
  semester INT,
  payment_date DATETIME,
  method ENUM('Online','Cash') DEFAULT 'Online',
  status ENUM('Paid','Pending') DEFAULT 'Pending',
  notes TEXT,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- News
CREATE TABLE news (
  id INT AUTO_INCREMENT PRIMARY KEY,
  title VARCHAR(255) NOT NULL,
  content TEXT NOT NULL,
  news_date DATE,
  created_by INT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (created_by) REFERENCES admins(id) ON DELETE SET NULL
);

-- Exams
CREATE TABLE exams (
  id INT AUTO_INCREMENT PRIMARY KEY,
  course_id INT NOT NULL,
  exam_type ENUM('Midterm','Final') NOT NULL,
  exam_date DATE NOT NULL,
  year INT NOT NULL,
  semester INT NOT NULL,
  duration_minutes INT,
  venue VARCHAR(255),
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Exam Results
CREATE TABLE exam_results (
  id INT AUTO_INCREMENT PRIMARY KEY,
  exam_id INT NOT NULL,
  student_id INT NOT NULL,
  degree DECIMAL(6,2),
  status ENUM('pass','fail','pmf','barred'),
  recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE (exam_id, student_id),
  FOREIGN KEY (exam_id) REFERENCES exams(id) ON DELETE CASCADE,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- PMD table
CREATE TABLE pmd_records (
  id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  course_id INT NOT NULL,
  pmd_status ENUM('PASS','MEDICAL','DELAYED') NOT NULL,
  notes TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
);

-- Audit log
CREATE TABLE audit_log (
  id BIGINT AUTO_INCREMENT PRIMARY KEY,
  actor_type VARCHAR(32), -- 'admin','doctor'
  actor_id INT,
  action VARCHAR(255),
  detail TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Indexes for performance
CREATE INDEX idx_students_code ON students(student_code);
CREATE INDEX idx_students_email ON students(email);
CREATE INDEX idx_enrollments_student ON enrollments(student_id);
CREATE INDEX idx_enrollments_course ON enrollments(course_id);
CREATE INDEX idx_grades_student ON grades(student_id);
CREATE INDEX idx_grades_course ON grades(course_id);
CREATE INDEX idx_attendance_student ON attendance(student_id);
CREATE INDEX idx_attendance_section ON attendance(section_id);
CREATE INDEX idx_payments_student ON payments(student_id);

