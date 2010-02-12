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