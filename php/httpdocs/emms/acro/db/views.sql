DROP VIEW IF EXISTS view_loan_status_max_id;
CREATE VIEW view_loan_status_max_id AS
  select
    max(lsh.id) id,
    lsh.loan_id
  from
    tblLoanStatusHistory lsh
  group by
    lsh.loan_id;

DROP VIEW IF EXISTS view_loan_status;
CREATE VIEW view_loan_status AS
  select
    lsm.loan_id loan_id,
    lsh.status status
  from
    tblLoanStatusHistory lsh, view_loan_status_max_id lsm
  where
    lsh.id = lsm.id;

DROP VIEW IF EXISTS view_sponsor_balance;
CREATE VIEW view_sponsor_balance AS
  select
    lm.sponsor_id,
    sum(lcd.balance_kp) balance
  from
    tblLoansCurrentData lcd,
    tblLoansMaster lm,
    tblLoansMasterDetails lmd
  where
    lmd.loan_id = lcd.loan_id and
    lm.id       = lmd.master_id
  group by
    lm.sponsor_id;

DROP VIEW IF EXISTS view_sponsor_write_off;
CREATE VIEW view_sponsor_write_off AS
  select
    lm.sponsor_id,
    sum(lwo.principal) write_off
  from
    tblLoanWriteOff lwo,
    tblLoansMaster lm,
    tblLoansMasterDetails lmd
  where
    lmd.loan_id = lwo.loan_id and
    lm.id       = lmd.master_id
  group by
    lm.sponsor_id;

DROP VIEW IF EXISTS view_loan_1st_time;
CREATE VIEW view_loan_1st_time AS
  select
    min(id) id
  from
    tblLoans
  group by
    client_id;