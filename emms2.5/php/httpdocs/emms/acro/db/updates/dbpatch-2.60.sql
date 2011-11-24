alter ignore table tblLoanWriteOff add unique index loan_id(loan_id);
alter ignore table tblLoanWriteOff add index user_id(user_id);

alter table tblCurrencys add column code tinytext null after symbol;

alter table tblSponsorsDonations add column src_tip decimal(10,2) after src_amount;
alter table tblSponsorsDonations add column conv_tip decimal(10,2) after conv_amount;
alter table tblSponsorsDonations add column token varchar(45) null after editor_date;
alter table tblSponsorsDonations add column memo text null after token;

alter table tblSponsors change column username username varchar(128) null default null;
alter table tblSponsors add column actcode varchar(128) after active;

drop table if exists tblCurrenciesExchangeRates;
create table tblCurrenciesExchangeRates (
  id int not null auto_increment,
  date date null,
  currency_id int null,
  rate decimal(10,2) null,
  primary key (id) ,
  index CurrencyID (currency_id asc) ,
  index Date (date asc) );

drop table if exists tblLoansMasterSponsors;
create table tblLoansMasterSponsors (
  id int not null auto_increment,
  master_id int null,
  sponsor_id int null,
  donation decimal(10,2) zerofill null,
  tip decimal(10,2) zerofill null,
  datetime datetime null,
  token varchar(45) null,
  primary key (id) ,
  index MasterID (master_id asc),
  index SponsorID (sponsor_id asc),
  unique Token (token) );


update tblCurrencys SET code='DOP' WHERE id='3';
update tblCurrencys SET code='USD' WHERE id='4';
update tblCurrencys SET code='EUR' WHERE id='5';

#if mondor script is not scheduled in the cronjob
#insert into tblCurrenciesExchangeRates (currency_id,date,rate) values (3,curdate(),38);

#update tblLoansMasterSponsors with historical data
insert into tblLoansMasterSponsors
  (master_id,sponsor_id,donation,tip,datetime)
select
  id,sponsor_id,amount,0,editor_date
from
  tblLoansMaster
where
  sponsor_id > 0 and editor_date <= date_sub(curdate(), INTERVAL 5 DAY);
