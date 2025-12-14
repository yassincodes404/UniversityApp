-- SQLite Database Schema for NCTU SIS - Comprehensive Version 2
-- Based on detailed requirements for New Cairo Technological University

PRAGMA foreign_keys = ON;

-- System Features
CREATE TABLE IF NOT EXISTS system_features (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT UNIQUE NOT NULL,
  title TEXT,
  description TEXT,
  required INTEGER DEFAULT 1,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Departments
CREATE TABLE IF NOT EXISTS departments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT UNIQUE,
  name TEXT NOT NULL,
  capacity INTEGER DEFAULT 0,
  notes TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Programs
CREATE TABLE IF NOT EXISTS programs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT NOT NULL UNIQUE,
  name TEXT NOT NULL,
  department_id INTEGER,
  degree_type TEXT,
  duration_years INTEGER DEFAULT 4,
  required_credits INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Roles
CREATE TABLE IF NOT EXISTS roles (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT UNIQUE NOT NULL
);

-- Permissions
CREATE TABLE IF NOT EXISTS permissions (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT UNIQUE NOT NULL,
  description TEXT
);

-- Role Permissions (many-to-many)
CREATE TABLE IF NOT EXISTS role_permissions (
  role_id INTEGER NOT NULL,
  permission_id INTEGER NOT NULL,
  PRIMARY KEY (role_id, permission_id),
  FOREIGN KEY (role_id) REFERENCES roles(id) ON DELETE CASCADE,
  FOREIGN KEY (permission_id) REFERENCES permissions(id) ON DELETE CASCADE
);

-- Students (must be created before users due to FK)
CREATE TABLE IF NOT EXISTS students (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_code TEXT UNIQUE NOT NULL,
  national_id TEXT,
  passport_no TEXT,
  first_name TEXT,
  middle_name TEXT,
  last_name TEXT,
  preferred_name TEXT,
  birth_date DATE,
  gender TEXT CHECK(gender IN ('M','F','Other')) DEFAULT 'Other',
  nationality TEXT DEFAULT 'Egyptian',
  profile_image_url TEXT,
  email TEXT,
  phone TEXT,
  address TEXT,
  entry_type TEXT CHECK(entry_type IN ('fresh','transfer')) DEFAULT 'fresh',
  previous_institution TEXT,
  previous_department_id INTEGER,
  enrollment_date DATE,
  current_level INTEGER DEFAULT 1,
  section_id INTEGER,
  program_id INTEGER,
  academic_status TEXT CHECK(academic_status IN ('active','probation','suspended','graduated')) DEFAULT 'active',
  credits_completed INTEGER DEFAULT 0,
  cgpa REAL DEFAULT 0.00,
  guardian_name TEXT,
  guardian_phone TEXT,
  military_status TEXT,
  admission_year INTEGER,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  updated_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE SET NULL,
  FOREIGN KEY (previous_department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Users (Authentication - separate from students/faculty)
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE NOT NULL,
  password_hash TEXT NOT NULL,
  salt TEXT NOT NULL,
  email TEXT,
  linked_student_id INTEGER,
  linked_faculty_id INTEGER,
  is_active INTEGER DEFAULT 1,
  must_change_password INTEGER DEFAULT 0,
  last_login TEXT,
  profile_image_url TEXT,
  profile_image_pending INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (linked_student_id) REFERENCES students(id) ON DELETE SET NULL,
  FOREIGN KEY (linked_faculty_id) REFERENCES faculty(id) ON DELETE SET NULL
);

-- User Roles (many-to-many)
CREATE TABLE IF NOT EXISTS user_roles (
  user_id INTEGER NOT NULL,
  role_id INTEGER NOT NULL,
  PRIMARY KEY (user_id, role_id),
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
  FOREIGN KEY (role_id) REFERENCES roles(id) ON DELETE CASCADE
);

-- Faculty
CREATE TABLE IF NOT EXISTS faculty (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  employee_code TEXT UNIQUE,
  title TEXT,
  first_name TEXT,
  last_name TEXT,
  email TEXT,
  phone TEXT,
  department_id INTEGER,
  is_course_coordinator INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Doctors (extends faculty)
CREATE TABLE IF NOT EXISTS doctors (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  faculty_id INTEGER NOT NULL UNIQUE,
  can_edit_grades INTEGER DEFAULT 1,
  notes TEXT,
  FOREIGN KEY (faculty_id) REFERENCES faculty(id) ON DELETE CASCADE
);

-- Sections (Academic sections within departments)
CREATE TABLE IF NOT EXISTS sections (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  department_id INTEGER,
  name TEXT NOT NULL,
  level_number INTEGER,
  max_students INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL
);

-- Rooms
CREATE TABLE IF NOT EXISTS rooms (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  type TEXT,
  capacity INTEGER,
  location TEXT
);

-- Labs
CREATE TABLE IF NOT EXISTS labs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  department_id INTEGER,
  room_id INTEGER,
  name TEXT,
  equipment_json TEXT,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL,
  FOREIGN KEY (room_id) REFERENCES rooms(id) ON DELETE SET NULL
);

-- Courses
CREATE TABLE IF NOT EXISTS courses (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_code TEXT UNIQUE NOT NULL,
  title TEXT NOT NULL,
  description TEXT,
  credits INTEGER NOT NULL DEFAULT 0,
  course_type TEXT CHECK(course_type IN ('Theory','Practical','Mixed')) DEFAULT 'Theory',
  department_id INTEGER,
  prerequisite_course_id INTEGER,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL,
  FOREIGN KEY (prerequisite_course_id) REFERENCES courses(id) ON DELETE SET NULL
);

-- Program Courses (which courses belong to which program and when)
CREATE TABLE IF NOT EXISTS program_courses (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  program_id INTEGER NOT NULL,
  course_id INTEGER NOT NULL,
  level_number INTEGER NOT NULL,
  semester INTEGER NOT NULL,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE CASCADE,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  UNIQUE (program_id, course_id, level_number, semester)
);

-- Course Instances (specific offering of a course in a term)
CREATE TABLE IF NOT EXISTS course_instances (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_id INTEGER NOT NULL,
  program_id INTEGER,
  year INTEGER NOT NULL,
  term INTEGER NOT NULL,
  section_id INTEGER,
  instructor_id INTEGER,
  capacity INTEGER,
  schedule_json TEXT,
  locked INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE SET NULL,
  FOREIGN KEY (instructor_id) REFERENCES faculty(id) ON DELETE SET NULL
);

-- Faculty Course Assignments
CREATE TABLE IF NOT EXISTS faculty_course_assignments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_instance_id INTEGER NOT NULL,
  faculty_id INTEGER NOT NULL,
  role TEXT DEFAULT 'instructor',
  FOREIGN KEY (course_instance_id) REFERENCES course_instances(id) ON DELETE CASCADE,
  FOREIGN KEY (faculty_id) REFERENCES faculty(id) ON DELETE CASCADE
);

-- Course Component Definitions (grading components for a course)
CREATE TABLE IF NOT EXISTS course_component_defs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_id INTEGER,
  course_instance_id INTEGER,
  component_code TEXT NOT NULL,
  max_score REAL NOT NULL DEFAULT 150.0,
  weight REAL,
  FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
  FOREIGN KEY (course_instance_id) REFERENCES course_instances(id) ON DELETE CASCADE
);

-- Course Enrollments
CREATE TABLE IF NOT EXISTS course_enrollments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  course_instance_id INTEGER NOT NULL,
  status TEXT CHECK(status IN ('registered','dropped','withdrawn','completed')) DEFAULT 'registered',
  registered_at TEXT DEFAULT CURRENT_TIMESTAMP,
  attempt_number INTEGER DEFAULT 1,
  row_version INTEGER DEFAULT 1,
  final_marks REAL,
  final_percent REAL,
  pmd_grade TEXT,
  rafeh_applied INTEGER DEFAULT 0,
  rafeh_amount REAL DEFAULT 0,
  attendance_percent REAL DEFAULT 0.0,
  attendance_total_sessions INTEGER DEFAULT 0,
  attendance_present_count INTEGER DEFAULT 0,
  barred_from_exam INTEGER DEFAULT 0,
  carry_over INTEGER DEFAULT 0,
  archived_at TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (course_instance_id) REFERENCES course_instances(id) ON DELETE CASCADE,
  UNIQUE (student_id, course_instance_id, attempt_number)
);

-- Grade Components (detailed grades for each component)
CREATE TABLE IF NOT EXISTS grade_components (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  enrollment_id INTEGER NOT NULL,
  component_code TEXT NOT NULL,
  max_score REAL NOT NULL DEFAULT 150.0,
  score REAL,
  recorded_at TEXT DEFAULT CURRENT_TIMESTAMP,
  recorded_by INTEGER,
  FOREIGN KEY (enrollment_id) REFERENCES course_enrollments(id) ON DELETE CASCADE,
  FOREIGN KEY (recorded_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Course Sessions (individual class meetings)
CREATE TABLE IF NOT EXISTS course_sessions (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  course_instance_id INTEGER NOT NULL,
  session_date TEXT NOT NULL,
  room_id INTEGER,
  session_type TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (course_instance_id) REFERENCES course_instances(id) ON DELETE CASCADE,
  FOREIGN KEY (room_id) REFERENCES rooms(id) ON DELETE SET NULL
);

-- Attendance Records
CREATE TABLE IF NOT EXISTS attendance_records (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  enrollment_id INTEGER NOT NULL,
  session_id INTEGER,
  session_date TEXT NOT NULL,
  status TEXT NOT NULL CHECK(status IN ('present','absent','late','excused')),
  remark TEXT,
  recorded_by INTEGER,
  recorded_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (enrollment_id) REFERENCES course_enrollments(id) ON DELETE CASCADE,
  FOREIGN KEY (session_id) REFERENCES course_sessions(id) ON DELETE SET NULL,
  FOREIGN KEY (recorded_by) REFERENCES users(id) ON DELETE SET NULL,
  UNIQUE (enrollment_id, session_date)
);

-- PMD Records (Pass/Medical/Deferred) - for assignments and exams
CREATE TABLE IF NOT EXISTS pmd_records (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  enrollment_id INTEGER NOT NULL,
  component_code TEXT NOT NULL,
  pmd_type TEXT CHECK(pmd_type IN ('PASS','MEDICAL','DEFERRED')) NOT NULL,
  doc_url TEXT,
  approved_by INTEGER,
  approved_at TEXT,
  notes TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (enrollment_id) REFERENCES course_enrollments(id) ON DELETE CASCADE,
  FOREIGN KEY (approved_by) REFERENCES users(id) ON DELETE SET NULL
);

-- PMD Rules (program-specific PMD configuration)
CREATE TABLE IF NOT EXISTS pmd_rules (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  program_id INTEGER,
  config_json TEXT,
  FOREIGN KEY (program_id) REFERENCES programs(id) ON DELETE CASCADE
);

-- Rafeh Adjustments (الرأفة)
CREATE TABLE IF NOT EXISTS rafeh_adjustments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  enrollment_id INTEGER NOT NULL,
  applied_by INTEGER,
  amount REAL NOT NULL,
  reason TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (enrollment_id) REFERENCES course_enrollments(id) ON DELETE CASCADE,
  FOREIGN KEY (applied_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Transcript Snapshots
CREATE TABLE IF NOT EXISTS transcript_snapshots (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  snapshot_json TEXT,
  pdf_url TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- Academic Progressions (level changes)
CREATE TABLE IF NOT EXISTS academic_progressions (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  from_level INTEGER,
  to_level INTEGER,
  status TEXT,
  reason TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- Student Transfers
CREATE TABLE IF NOT EXISTS student_transfers (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  from_program_id INTEGER,
  from_department_id INTEGER,
  from_section_id INTEGER,
  to_program_id INTEGER,
  to_department_id INTEGER,
  to_section_id INTEGER,
  transfer_type TEXT NOT NULL,
  reason TEXT,
  effective_date TEXT,
  requested_by INTEGER,
  approved_by INTEGER,
  status TEXT CHECK(status IN ('pending','approved','rejected')) DEFAULT 'pending',
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (from_program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (to_program_id) REFERENCES programs(id) ON DELETE SET NULL,
  FOREIGN KEY (from_department_id) REFERENCES departments(id) ON DELETE SET NULL,
  FOREIGN KEY (to_department_id) REFERENCES departments(id) ON DELETE SET NULL,
  FOREIGN KEY (from_section_id) REFERENCES sections(id) ON DELETE SET NULL,
  FOREIGN KEY (to_section_id) REFERENCES sections(id) ON DELETE SET NULL,
  FOREIGN KEY (requested_by) REFERENCES users(id) ON DELETE SET NULL,
  FOREIGN KEY (approved_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Credit Equivalences (transfer credits)
CREATE TABLE IF NOT EXISTS credit_equivalences (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  external_course_code TEXT,
  external_institution TEXT,
  accepted_credit INTEGER,
  accepted_course_id INTEGER,
  notes TEXT,
  approved_by INTEGER,
  approved_at TEXT,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
  FOREIGN KEY (accepted_course_id) REFERENCES courses(id) ON DELETE SET NULL,
  FOREIGN KEY (approved_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Section Waitlist
CREATE TABLE IF NOT EXISTS section_waitlist (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  section_id INTEGER NOT NULL,
  student_id INTEGER NOT NULL,
  priority INTEGER DEFAULT 0,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- Documents
CREATE TABLE IF NOT EXISTS documents (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  owner_type TEXT,
  owner_id INTEGER,
  doc_type TEXT,
  file_url TEXT,
  file_hash TEXT,
  uploaded_by INTEGER,
  uploaded_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (uploaded_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Payments
CREATE TABLE IF NOT EXISTS payments (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  amount REAL,
  payment_type TEXT CHECK(payment_type IN ('tuition','scholarship','other')) DEFAULT 'tuition',
  semester INTEGER,
  payment_date TEXT,
  method TEXT CHECK(method IN ('Online','Cash','Bank')) DEFAULT 'Online',
  status TEXT CHECK(status IN ('Paid','Pending','Failed')) DEFAULT 'Pending',
  receipt_url TEXT,
  notes TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- Training Records (Internships/Field Training)
CREATE TABLE IF NOT EXISTS training_records (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  student_id INTEGER NOT NULL,
  company_name TEXT,
  start_date DATE,
  end_date DATE,
  supervisor_name TEXT,
  supervisor_email TEXT,
  report_url TEXT,
  grade REAL,
  evaluation TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE
);

-- News/Announcements
CREATE TABLE IF NOT EXISTS news (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  title TEXT NOT NULL,
  content TEXT NOT NULL,
  news_date DATE,
  created_by INTEGER,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (created_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Audit Logs
CREATE TABLE IF NOT EXISTS audit_logs (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id INTEGER,
  action TEXT NOT NULL,
  target_table TEXT NOT NULL,
  target_id INTEGER,
  diff_json TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL
);

-- Profile Change Requests (for student profile image changes)
CREATE TABLE IF NOT EXISTS profile_change_requests (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id INTEGER NOT NULL,
  request_type TEXT CHECK(request_type IN ('profile_image','email','phone','address')) NOT NULL,
  old_value TEXT,
  new_value TEXT,
  status TEXT CHECK(status IN ('pending','approved','rejected')) DEFAULT 'pending',
  requested_at TEXT DEFAULT CURRENT_TIMESTAMP,
  reviewed_by INTEGER,
  reviewed_at TEXT,
  review_notes TEXT,
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
  FOREIGN KEY (reviewed_by) REFERENCES users(id) ON DELETE SET NULL
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_students_code ON students(student_code);
CREATE INDEX IF NOT EXISTS idx_students_national_id ON students(national_id);
CREATE INDEX IF NOT EXISTS idx_users_username ON users(username);
CREATE INDEX IF NOT EXISTS idx_users_linked_student ON users(linked_student_id);
CREATE INDEX IF NOT EXISTS idx_users_linked_faculty ON users(linked_faculty_id);
CREATE INDEX IF NOT EXISTS idx_course_enrollments_student ON course_enrollments(student_id);
CREATE INDEX IF NOT EXISTS idx_course_enrollments_instance ON course_enrollments(course_instance_id);
CREATE INDEX IF NOT EXISTS idx_grade_components_enrollment ON grade_components(enrollment_id);
CREATE INDEX IF NOT EXISTS idx_attendance_enrollment ON attendance_records(enrollment_id);
CREATE INDEX IF NOT EXISTS idx_pmd_enrollment ON pmd_records(enrollment_id);
CREATE INDEX IF NOT EXISTS idx_course_instances_course ON course_instances(course_id);
CREATE INDEX IF NOT EXISTS idx_course_instances_program ON course_instances(program_id);
CREATE INDEX IF NOT EXISTS idx_program_courses_program ON program_courses(program_id);
CREATE INDEX IF NOT EXISTS idx_program_courses_course ON program_courses(course_id);
CREATE INDEX IF NOT EXISTS idx_audit_logs_user ON audit_logs(user_id);
CREATE INDEX IF NOT EXISTS idx_audit_logs_target ON audit_logs(target_table, target_id);
