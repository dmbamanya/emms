alter ignore table tblUsers add unique index `username`(`username`);
alter ignore table tblLoanStatusHistory add unique index `nodups`(`loan_id`,`p_status`,`status`,`date`);
drop view if exists view_loan_status_dates;
create view view_loan_status_dates as
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