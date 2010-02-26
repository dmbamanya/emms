truncate tblTCredits;

insert
  into
    tblTCredits
    (
      code,
      date,
      branch_id,
      program_id,
      fund_id,
      amount,
      principal,
      fees,
      insurances,
      interest,
      penalties
    )
  select
    CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0'),LPAD(flmp.fund_id,3,'0')) as code,
    p.date            date,
    l.zone_id         branch_id,
    l.program_id      program_id,
    flmp.fund_id      fund_id,
    sum(p.pmt)        amount,
    sum(p.principal)  principal,
    sum(p.fees)       fees,
    sum(p.insurances) insurances,
    sum(p.interest)   interest,
    sum(p.penalties)  penalties
  from
    tblPayments p,
    tblLoans l,
    tblFundsLoansMasterPct flmp,
    tblLoansMasterDetails lmd
  where
    lmd.loan_id = l.id             and
    flmp.master_id = lmd.master_id and
    l.id = p.loan_id
  group by
    code;

