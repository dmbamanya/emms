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

DROP VIEW IF EXISTS view_loan_status_dates;
CREATE VIEW view_loan_status_dates AS
  select
    l.id,
    l.creator_date created,
    o.date open,
    r.date rejected,
    ro.date re_open,
    s.date revised,
    a.date approved,
    d.date disbursed,
    g.date active,
    l.xp_cancel_date xp_cancel,
    c.date cancelled,
    li.date legal_in,
    lo.date legal_out,
    rt.date retracted
  from tblLoans l
  left join tblLoanStatusHistory o on o.loan_id = l.id and o.status = 'O'
  left join tblLoanStatusHistory s on s.loan_id = l.id and s.status = 'S'
  left join tblLoanStatusHistory a on a.loan_id = l.id and a.status = 'A'
  left join tblLoanStatusHistory d on d.loan_id = l.id and d.status = 'D'
  left join tblLoanStatusHistory g on g.loan_id = l.id and g.status = 'G'
  left join tblLoanStatusHistory c on c.loan_id = l.id and c.status = 'C'
  left join tblLoanStatusHistory li on li.loan_id = l.id and li.status = 'LI'
  left join tblLoanStatusHistory lo on lo.loan_id = l.id and lo.status = 'LO'
  left join tblLoanStatusHistory rt on rt.loan_id = l.id and rt.status = 'RT'
  left join tblLoanStatusHistory ro on ro.loan_id = l.id and ro.status = 'RO'
  left join tblLoanStatusHistory r on r.loan_id = l.id and r.status = 'R';

drop view if exists view_receipt_flags;
create view view_receipt_flags as
  select
    p.loan_id,p.id payment_id, p.date, lrp.receipt_id,r.flag_a,r.flag_b
  from
    tblPayments p, tblLinkReceiptsPayments lrp, tblReceipts r
  where
    lrp.payment_id = p.id and r.id = lrp.receipt_id;

drop view if exists view_loan_funds;
create view view_loan_funds as
  select
    l.id,f.id fund_id,f.fund
  from
    tblLoans l, tblFunds f, tblLoansMasterDetails lmd, tblFundsLoansMasterPct flmp
  where
    lmd.loan_id = l.id and
    flmp.master_id = lmd.master_id and
    f.id = flmp.fund_id;

drop view if exists view_loan_group;
create view view_loan_group as
select
  l.id, s.id group_id, s.name group_name
from
  (
    tblLoans l,
    tblLoansMasterDetails lmd,
    tblLoansMaster lm
  )
left join
  tblSocieties s on s.id = lm.borrower_id and lm.borrower_type = 'B'
where
  lmd.loan_id = l.id and
  lm.id = lmd.master_id;