alter table tblLoansMaster modify column check_status enum('P','A','D','R','RT') not null default 'P';
alter table tblLoansMasterTrash modify column check_status enum('P','A','D','R','RT') not null default 'P';
update 
  tblLoansMaster lm
set 
  lm.check_status = 'RT'
where
  lm.id in
  (
    select
      distinct lmd.master_id
    from
      tblloans l,
      tblLoansMasterDetails lmd
    where
      lmd.loan_id = l.id and
      l.status = 'RT'
   );