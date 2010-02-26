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