DROP TABLE IF EXISTS `tblBusiness`;
CREATE TABLE  `tblBusiness` (
  `id` int(11) NOT NULL auto_increment,
  `type_id` int(11) default NULL,
  `name` varchar(255) NOT NULL default '',
  `client_list` varchar(255) NOT NULL default '',
  `description` text,
  `status` enum('N','A','C','L') default NULL,
  `creator_date` date default NULL,
  `creator_id` int(11) default NULL,
  `editor_date` date default NULL,
  `editor_id` int(11) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblBusinessIOM`;
CREATE TABLE  `tblBusinessIOM` (
  `id` int(11) NOT NULL auto_increment,
  `business_id` int(11) NOT NULL default '0',
  `status` char(1) NOT NULL default 'N',
  `creator_date` date NOT NULL default '0000-00-00',
  `creator_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblBusinessTypes`;
CREATE TABLE  `tblBusinessTypes` (
  `id` int(11) NOT NULL auto_increment,
  `activity` enum('S','I','C','F') default NULL,
  `type` tinytext,
  `description` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblCalendar`;
CREATE TABLE  `tblCalendar` (
  `date` date NOT NULL default '0000-00-00'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='RP';

DROP TABLE IF EXISTS `tblClientIOM`;
CREATE TABLE  `tblClientIOM` (
  `id` int(11) NOT NULL auto_increment,
  `internal` enum('0','1') NOT NULL default '0',
  `type` enum('I','O') default NULL,
  `client_id` int(11) default NULL,
  `society_id` int(11) default NULL,
  `advisor_id` int(11) default NULL,
  `zone_id` tinyint(4) NOT NULL default '0',
  `date` date default NULL,
  `cause` enum('A','B','C','D','E','F','G','H','I','J') default NULL,
  `user_id` int(11) default NULL,
  `memo` text,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblClientPortfolio`;
CREATE TABLE  `tblClientPortfolio` (
  `date` date NOT NULL default '0000-00-00',
  `zone_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) NOT NULL default '0',
  `clients` int(11) NOT NULL default '0',
  `female` int(11) NOT NULL default '0',
  `male` int(11) NOT NULL default '0',
  `client_i` int(11) NOT NULL default '0',
  `client_g` int(11) NOT NULL default '0',
  `client_b` int(11) NOT NULL default '0',
  `group_g` int(11) NOT NULL default '0',
  `group_b` int(11) NOT NULL default '0',
  `group_bg` int(11) NOT NULL default '0',
  `client_al` int(11) NOT NULL default '0',
  UNIQUE KEY `date` (`date`,`zone_id`,`program_id`,`advisor_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

DROP TABLE IF EXISTS `tblClients`;
CREATE TABLE  `tblClients` (
  `id` int(11) NOT NULL auto_increment,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  `code` varchar(13) default NULL,
  `zone_id` tinyint(4) default NULL,
  `first` varchar(16) default NULL,
  `middle` varchar(16) default NULL,
  `last` varchar(16) default NULL,
  `nick` varchar(16) default NULL,
  `skills` varchar(32) default NULL,
  `education` enum('N','SP','P','SH','H','SC','C','SU','U','G') default NULL,
  `cstatus` enum('M','S','E','D','W') default NULL,
  `spouse` varchar(32) default NULL,
  `dependants` tinyint(2) default NULL,
  `birthdate` date default NULL,
  `gender` enum('M','F') default NULL,
  `phone` varchar(14) default NULL,
  `mobile` varchar(14) default NULL,
  `email` varchar(64) default NULL,
  `address` text,
  `society_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) default NULL,
  `memo` text,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `society_id` (`society_id`),
  FULLTEXT KEY `code` (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblContracts`;
CREATE TABLE  `tblContracts` (
  `id` int(11) NOT NULL auto_increment,
  `tpl` varchar(64) NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` varchar(64) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tblCurrencys`;
CREATE TABLE  `tblCurrencys` (
  `id` int(11) NOT NULL auto_increment,
  `currency` tinytext,
  `symbol` tinytext,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblDataLog`;
CREATE TABLE  `tblDataLog` (
  `id` int(11) NOT NULL auto_increment,
  `date` date NOT NULL default '0000-00-00',
  `time` time NOT NULL default '00:00:00',
  `mode` varchar(255) NOT NULL default '',
  `script` varchar(255) NOT NULL default '',
  `description` varchar(255) NOT NULL default '',
  `user_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblFunds`;
CREATE TABLE  `tblFunds` (
  `id` int(11) NOT NULL auto_increment,
  `code` varchar(32) default NULL,
  `fund` varchar(32) default NULL,
  `currency_id` int(11) NOT NULL default '1',
  `description` varchar(255) default NULL,
  `status` enum('A','I') default NULL,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Accounting';

DROP TABLE IF EXISTS `tblFundsLoansMasterPct`;
CREATE TABLE  `tblFundsLoansMasterPct` (
  `id` int(11) NOT NULL auto_increment,
  `master_id` int(11) NOT NULL default '0',
  `fund_id` int(11) NOT NULL default '0',
  `pct` int(3) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblFundsLoansMasterPctTrash`;
CREATE TABLE  `tblFundsLoansMasterPctTrash` (
  `id` int(11) NOT NULL,
  `master_id` int(11) NOT NULL default '0',
  `fund_id` int(11) NOT NULL default '0',
  `pct` int(3) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblGetText`;
CREATE TABLE  `tblGetText` (
  `id` varchar(255) NOT NULL,
  `eng` text NOT NULL,
  `esp` text NOT NULL,
  `fra` text NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tblLinkProgramsFunds`;
CREATE TABLE  `tblLinkProgramsFunds` (
  `program_id` int(11) NOT NULL default '0',
  `fund_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`program_id`,`fund_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblLinkReceiptsPayments`;
CREATE TABLE  `tblLinkReceiptsPayments` (
  `receipt_id` int(10) unsigned NOT NULL,
  `payment_id` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`payment_id`,`receipt_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblLinkSocieties`;
CREATE TABLE  `tblLinkSocieties` (
  `parent_id` int(11) NOT NULL default '0',
  `child_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`parent_id`,`child_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblLinkSponsorsZones`;
CREATE TABLE  `tblLinkSponsorsZones` (
  `sponsor_id` int(11) NOT NULL,
  `zone_id` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblLoans`;
CREATE TABLE  `tblLoans` (
  `id` int(11) NOT NULL auto_increment,
  `loan_code` tinytext NOT NULL,
  `status` enum('N','R','O','S','A','D','G','C','LI','LO','RT','RO') default NULL,
  `client_id` int(11) NOT NULL default '0',
  `loan_type_id` tinyint(4) NOT NULL default '0',
  `business_id` int(11) NOT NULL default '0',
  `installment` int(11) NOT NULL default '0',
  `fees_at` float(5,2) NOT NULL default '0.00',
  `fees_af` float(5,2) NOT NULL default '0.00',
  `rates_r` decimal(4,2) NOT NULL default '0.00',
  `rates_d` decimal(4,2) NOT NULL default '0.00',
  `rates_e` decimal(5,2) NOT NULL default '0.00',
  `margin_d` tinyint(4) NOT NULL default '0',
  `kp` decimal(10,2) NOT NULL default '0.00',
  `kat` decimal(10,2) NOT NULL default '0.00',
  `kaf` decimal(10,2) NOT NULL default '0.00',
  `pmt` decimal(10,2) NOT NULL default '0.00',
  `savings_p` decimal(10,2) NOT NULL default '0.00',
  `savings_v` decimal(10,2) NOT NULL default '0.00',
  `pg_value` decimal(10,2) NOT NULL default '0.00',
  `pg_memo` tinytext NOT NULL,
  `re_value` decimal(10,2) NOT NULL default '0.00',
  `re_memo` tinytext NOT NULL,
  `fgd_value` decimal(10,2) NOT NULL default '0.00',
  `fgd_memo` tinytext NOT NULL,
  `fgt_value` decimal(10,2) NOT NULL default '0.00',
  `fgt_memo` tinytext NOT NULL,
  `zone_id` int(11) NOT NULL default '0',
  `client_zone_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) NOT NULL default '0',
  `creator_date` date default NULL,
  `creator_id` int(11) default NULL,
  `editor_date` date default NULL,
  `editor_id` int(11) NOT NULL default '0',
  `delivered_date` date NOT NULL default '0000-00-00',
  `first_payment_date` date NOT NULL default '0000-00-00',
  `xp_cancel_date` date NOT NULL,
  `xp_num_pmt` int(11) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `client_id` (`client_id`),
  KEY `delivered_date` (`delivered_date`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansCurrentData`;
CREATE TABLE  `tblLoansCurrentData` (
  `loan_id` int(11) NOT NULL default '0',
  `balance_kp` decimal(10,2) NOT NULL default '0.00',
  `balance_kaf` decimal(10,2) NOT NULL default '0.00',
  `balance_kat` decimal(10,2) NOT NULL default '0.00',
  `r_from_date` date NOT NULL default '0000-00-00',
  `xp_pmt_date` date NOT NULL default '0000-00-00',
  `xp_pmt` decimal(10,2) NOT NULL default '0.00',
  `cn_date` date NOT NULL default '0000-00-00',
  `cn_delay` int(11) NOT NULL default '0',
  `cn_penalties` decimal(10,2) NOT NULL default '0.00',
  PRIMARY KEY  (`loan_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansCurrentDataBackup`;
CREATE TABLE  `tblLoansCurrentDataBackup` (
  `loan_id` int(11) NOT NULL default '0',
  `balance_kp` decimal(10,2) NOT NULL default '0.00',
  `balance_kaf` decimal(10,2) NOT NULL default '0.00',
  `balance_kat` decimal(10,2) NOT NULL default '0.00',
  `r_from_date` date NOT NULL default '0000-00-00',
  `xp_pmt_date` date NOT NULL default '0000-00-00',
  `xp_pmt` decimal(10,2) NOT NULL default '0.00',
  `cn_date` date NOT NULL default '0000-00-00',
  `cn_delay` int(11) NOT NULL default '0',
  `cn_penalties` decimal(10,2) NOT NULL default '0.00',
  PRIMARY KEY  (`loan_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansMaster`;
CREATE TABLE  `tblLoansMaster` (
  `id` int(11) NOT NULL auto_increment,
  `borrower_id` int(11) NOT NULL default '0',
  `borrower_type` enum('B','G','I') collate latin1_general_ci NOT NULL default 'B',
  `loan_type_id` int(11) NOT NULL default '0',
  `amount` decimal(13,2) NOT NULL default '0.00',
  `check_number` varchar(128) collate latin1_general_ci NOT NULL default '',
  `check_status` enum('P','A','D','R','RT') collate latin1_general_ci NOT NULL default 'P',
  `program_id` int(11) NOT NULL default '0',
  `zone_id` int(11) NOT NULL default '0',
  `creator_id` int(11) NOT NULL default '0',
  `creator_date` date NOT NULL default '0000-00-00',
  `editor_id` int(11) NOT NULL default '0',
  `editor_date` date NOT NULL default '0000-00-00',
  `xp_delivered_date` date NOT NULL,
  `xp_first_payment_date` date NOT NULL,
  `chk_process` int(11) NOT NULL default '0',
  `sponsor_id` int(11) default '0',
  `kiva_id` int(11) default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansMasterDetails`;
CREATE TABLE  `tblLoansMasterDetails` (
  `master_id` int(11) NOT NULL default '0',
  `loan_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`master_id`,`loan_id`),
  KEY `loan_id` (`loan_id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansMasterDetailsTrash`;
CREATE TABLE  `tblLoansMasterDetailsTrash` (
  `master_id` int(11) NOT NULL default '0',
  `loan_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`master_id`,`loan_id`),
  KEY `loan_id` (`loan_id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansMasterTrash`;
CREATE TABLE  `tblLoansMasterTrash` (
  `id` int(11) NOT NULL,
  `borrower_id` int(11) NOT NULL default '0',
  `borrower_type` enum('B','G','I') collate latin1_general_ci NOT NULL default 'B',
  `loan_type_id` int(11) NOT NULL default '0',
  `amount` decimal(13,2) NOT NULL default '0.00',
  `check_number` varchar(128) collate latin1_general_ci NOT NULL default '',
  `check_status` enum('P','A','D','R','RT') collate latin1_general_ci NOT NULL default 'P',
  `program_id` int(11) NOT NULL default '0',
  `zone_id` int(11) NOT NULL default '0',
  `creator_id` int(11) NOT NULL default '0',
  `creator_date` date NOT NULL default '0000-00-00',
  `editor_id` int(11) NOT NULL default '0',
  `editor_date` date NOT NULL default '0000-00-00',
  `xp_delivered_date` date NOT NULL,
  `xp_first_payment_date` date NOT NULL,
  `chk_process` int(11) NOT NULL default '0',
  `sponsor_id` int(11) default '0',
  `kiva_id` int(11) default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansOnDelinquency`;
CREATE TABLE  `tblLoansOnDelinquency` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL default '0',
  `date` date NOT NULL default '0000-00-00',
  `hits` int(11) NOT NULL default '0',
  `delay` int(11) NOT NULL default '0',
  `pmt` decimal(13,2) NOT NULL default '0.00',
  `penalties` decimal(13,2) NOT NULL default '0.00',
  `interest` decimal(13,2) NOT NULL default '0.00',
  `fees` decimal(13,2) NOT NULL default '0.00',
  `insurances` decimal(13,2) NOT NULL default '0.00',
  `principal` decimal(13,2) NOT NULL default '0.00',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `loan_id` (`loan_id`,`date`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='CRON';

DROP TABLE IF EXISTS `tblLoansParked`;
CREATE TABLE  `tblLoansParked` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL,
  `category` enum('disaster','tdisability','pdisability','death','other') NOT NULL,
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL,
  `principal` decimal(10,2) NOT NULL,
  `insurance` decimal(10,2) NOT NULL,
  `fees` decimal(10,2) NOT NULL,
  `interest` decimal(10,2) NOT NULL,
  `penalties` decimal(10,2) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `loan_id` (`loan_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansParkedPayments`;
CREATE TABLE  `tblLoansParkedPayments` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL,
  `principal` decimal(10,2) NOT NULL,
  `insurance` decimal(10,2) NOT NULL,
  `fees` decimal(10,2) NOT NULL,
  `interest` decimal(10,2) NOT NULL,
  `penalties` decimal(10,2) NOT NULL,
  `total` decimal(10,2) NOT NULL,
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanStatusHistory`;
CREATE TABLE  `tblLoanStatusHistory` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL default '0',
  `p_status` varchar(16) NOT NULL default '',
  `status` varchar(16) NOT NULL default '',
  `date` date NOT NULL default '0000-00-00',
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `user_id` int(11) NOT NULL default '0',
  `memo` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `loan_id` (`loan_id`),
  KEY `date` (`date`),
  KEY `status` (`status`),
  KEY `p_status` (`p_status`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanStatusHistoryTrash`;
CREATE TABLE  `tblLoanStatusHistoryTrash` (
  `id` int(11) NOT NULL,
  `loan_id` int(11) NOT NULL default '0',
  `p_status` varchar(16) NOT NULL default '',
  `status` varchar(16) NOT NULL default '',
  `date` date NOT NULL default '0000-00-00',
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `user_id` int(11) NOT NULL default '0',
  `memo` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `loan_id` (`loan_id`),
  KEY `date` (`date`),
  KEY `status` (`status`),
  KEY `p_status` (`p_status`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansTrash`;
CREATE TABLE  `tblLoansTrash` (
  `id` int(11) NOT NULL,
  `loan_code` tinytext NOT NULL,
  `status` enum('N','R','O','S','A','D','G','C','LI','LO','RT','RO') default NULL,
  `client_id` int(11) NOT NULL default '0',
  `loan_type_id` tinyint(4) NOT NULL default '0',
  `business_id` int(11) NOT NULL default '0',
  `installment` int(11) NOT NULL default '0',
  `fees_at` float(5,2) NOT NULL default '0.00',
  `fees_af` float(5,2) NOT NULL default '0.00',
  `rates_r` decimal(4,2) NOT NULL default '0.00',
  `rates_d` decimal(4,2) NOT NULL default '0.00',
  `rates_e` decimal(5,2) NOT NULL default '0.00',
  `margin_d` tinyint(4) NOT NULL default '0',
  `kp` decimal(10,2) NOT NULL default '0.00',
  `kat` decimal(10,2) NOT NULL default '0.00',
  `kaf` decimal(10,2) NOT NULL default '0.00',
  `pmt` decimal(10,2) NOT NULL default '0.00',
  `savings_p` decimal(10,2) NOT NULL default '0.00',
  `savings_v` decimal(10,2) NOT NULL default '0.00',
  `pg_value` decimal(10,2) NOT NULL default '0.00',
  `pg_memo` tinytext NOT NULL,
  `re_value` decimal(10,2) NOT NULL default '0.00',
  `re_memo` tinytext NOT NULL,
  `fgd_value` decimal(10,2) NOT NULL default '0.00',
  `fgd_memo` tinytext NOT NULL,
  `fgt_value` decimal(10,2) NOT NULL default '0.00',
  `fgt_memo` tinytext NOT NULL,
  `zone_id` int(11) NOT NULL default '0',
  `client_zone_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) NOT NULL default '0',
  `creator_date` date default NULL,
  `creator_id` int(11) default NULL,
  `editor_date` date default NULL,
  `editor_id` int(11) NOT NULL default '0',
  `delivered_date` date NOT NULL default '0000-00-00',
  `first_payment_date` date NOT NULL default '0000-00-00',
  `xp_cancel_date` date NOT NULL,
  `xp_num_pmt` int(11) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `client_id` (`client_id`),
  KEY `delivered_date` (`delivered_date`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanTypes`;
CREATE TABLE  `tblLoanTypes` (
  `id` int(11) NOT NULL auto_increment,
  `borrower_type` enum('B','G','I','U','O') NOT NULL default 'B',
  `description` tinytext,
  `installment` int(11) default NULL,
  `installment_lock` enum('1','0') NOT NULL default '1',
  `rates_r` float(5,2) NOT NULL default '0.00',
  `rates_r_lock` enum('1','0') NOT NULL default '1',
  `rates_d` float(5,2) NOT NULL default '0.00',
  `rates_d_lock` enum('1','0') NOT NULL default '1',
  `fees_a` float(5,2) NOT NULL default '0.00',
  `fees_at` float(5,2) NOT NULL default '0.00',
  `fees_at_lock` enum('1','0') NOT NULL default '1',
  `fees_af` float(5,2) NOT NULL default '0.00',
  `fees_af_lock` enum('1','0') NOT NULL default '1',
  `margin_r` tinyint(4) default '0',
  `margin_k` tinyint(4) default '0',
  `margin_d` tinyint(4) default '0',
  `margin_d_lock` enum('1','0') NOT NULL default '1',
  `payment_frequency` enum('W','BW','M','Q','SA','A') default NULL,
  `calendar_type` enum('360','365') default NULL,
  `margin_c` float(5,2) NOT NULL default '0.00',
  `margin_c_lock` enum('1','0') NOT NULL default '1',
  `savings_p` decimal(5,2) NOT NULL default '0.00',
  `savings_p_lock` enum('1','0') NOT NULL default '1',
  `currency_id` tinyint(4) NOT NULL default '3',
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanWriteOff`;
CREATE TABLE  `tblLoanWriteOff` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL default '0',
  `amount` decimal(10,2) NOT NULL default '0.00',
  `principal` decimal(10,2) NOT NULL,
  `insurance` decimal(10,2) NOT NULL,
  `fees` decimal(10,2) NOT NULL,
  `interest` decimal(10,2) NOT NULL,
  `penalties` decimal(10,2) NOT NULL,
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanWriteOffCharges`;
CREATE TABLE  `tblLoanWriteOffCharges` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL,
  `amount` decimal(10,2) NOT NULL,
  `description` varchar(255) NOT NULL,
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tblLoanWriteOffPayments`;
CREATE TABLE  `tblLoanWriteOffPayments` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) NOT NULL,
  `amount` decimal(10,2) NOT NULL,
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblPasswords`;
CREATE TABLE  `tblPasswords` (
  `user_id` int(11) NOT NULL,
  `date` date NOT NULL,
  `password` varchar(255) collate latin1_general_ci NOT NULL,
  PRIMARY KEY  USING BTREE (`user_id`,`date`),
  KEY `user_id` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='BASE';

DROP TABLE IF EXISTS `tblPayments`;
CREATE TABLE  `tblPayments` (
  `id` int(11) NOT NULL auto_increment,
  `loan_id` int(11) default NULL,
  `date` date default NULL,
  `pmt` decimal(10,2) default NULL,
  `penalties` decimal(10,2) default NULL,
  `delay` int(11) NOT NULL default '0',
  `interest` decimal(10,2) default NULL,
  `insurances` decimal(10,2) NOT NULL default '0.00',
  `fees` decimal(10,2) NOT NULL default '0.00',
  `principal` decimal(10,2) default NULL,
  `balance_kp` decimal(10,2) default NULL,
  `balance_kaf` decimal(10,2) NOT NULL default '0.00',
  `balance_kat` decimal(10,2) NOT NULL default '0.00',
  `special` smallint(6) NOT NULL default '0',
  `user_id` int(11) default NULL,
  `transaction_id` varchar(18) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `loan_id` (`loan_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblPaymentsRollback`;
CREATE TABLE  `tblPaymentsRollback` (
  `id` int(11) NOT NULL default '0',
  `loan_id` int(11) default NULL,
  `date` date default NULL,
  `pmt` decimal(10,2) default NULL,
  `penalties` decimal(10,2) default NULL,
  `delay` int(11) NOT NULL default '0',
  `interest` decimal(10,2) default NULL,
  `insurances` decimal(10,2) NOT NULL default '0.00',
  `fees` decimal(10,2) NOT NULL default '0.00',
  `principal` decimal(10,2) default NULL,
  `balance_kp` decimal(10,2) default NULL,
  `balance_kaf` decimal(10,2) NOT NULL default '0.00',
  `balance_kat` decimal(10,2) NOT NULL default '0.00',
  `special` smallint(6) NOT NULL default '0',
  `user_id` int(11) default NULL,
  `transaction_id` varchar(18) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblPrograms`;
CREATE TABLE  `tblPrograms` (
  `id` int(11) NOT NULL auto_increment,
  `program` varchar(32) default NULL,
  `confidential` enum('1','0') NOT NULL default '0',
  `description` varchar(255) default NULL,
  `status` enum('A','I') default NULL,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='BASE';

DROP TABLE IF EXISTS `tblQjumps`;
CREATE TABLE  `tblQjumps` (
  `user_id` int(11) NOT NULL default '0',
  `qjump` varchar(255) collate latin1_general_ci NOT NULL default '',
  PRIMARY KEY  (`user_id`,`qjump`),
  KEY `user_id` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='BASE';

DROP TABLE IF EXISTS `tblReceipts`;
CREATE TABLE  `tblReceipts` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `loanmaster_id` int(10) unsigned NOT NULL,
  `balance_kp` decimal(10,2) NOT NULL,
  `balance_kaf` decimal(10,2) NOT NULL,
  `balance_kat` decimal(10,2) NOT NULL,
  `notes` tinytext NOT NULL,
  `flag_a` enum('0','1') NOT NULL default '0',
  `flag_b` enum('0','1') NOT NULL default '0',
  `user_id` int(10) unsigned NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tblRiskPortfolio`;
CREATE TABLE  `tblRiskPortfolio` (
  `date` date NOT NULL default '0000-00-00',
  `zone_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) NOT NULL default '0',
  `balance` decimal(16,2) NOT NULL default '0.00',
  `riskA` decimal(16,2) NOT NULL default '0.00',
  `riskB` decimal(16,2) NOT NULL default '0.00',
  `riskC` decimal(16,2) NOT NULL default '0.00',
  `riskD` decimal(16,2) NOT NULL default '0.00',
  UNIQUE KEY `date` (`date`,`zone_id`,`program_id`,`advisor_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

DROP TABLE IF EXISTS `tblSocieties`;
CREATE TABLE  `tblSocieties` (
  `id` int(11) NOT NULL auto_increment,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  `code` varchar(13) default NULL,
  `category` enum('B','G','BG') default NULL,
  `name` varchar(64) default NULL,
  `zone_id` tinyint(4) default NULL,
  `advisor_id` int(11) default NULL,
  `president_id` int(11) default NULL,
  `treasurer_id` int(11) default NULL,
  `secretary_id` int(11) default NULL,
  `memo` text,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSponsors`;
CREATE TABLE  `tblSponsors` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(16) default NULL,
  `password` varchar(35) default NULL,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  `first` varchar(128) default NULL,
  `middle` varchar(128) default NULL,
  `last` varchar(128) default NULL,
  `sponsor` varchar(255) default NULL,
  `email` varchar(128) default NULL,
  `memo` text,
  `active` enum('1','0') NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSponsorsDonations`;
CREATE TABLE  `tblSponsorsDonations` (
  `id` int(11) NOT NULL auto_increment,
  `sponsor_id` int(11) default NULL,
  `new_clients` enum('0','1') NOT NULL default '0',
  `src_amount` decimal(20,2) default NULL,
  `src_currency_id` int(11) default NULL,
  `conv_amount` decimal(20,2) default NULL,
  `conv_currency_id` int(11) default NULL,
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSponsorsLog`;
CREATE TABLE  `tblSponsorsLog` (
  `id` int(11) NOT NULL auto_increment,
  `sponsor_id` int(11) default NULL,
  `ip_address` varchar(15) default NULL,
  `login_date` datetime default NULL,
  `last_hit_date` datetime default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSurveyAnswers`;
CREATE TABLE  `tblSurveyAnswers` (
  `id` int(11) NOT NULL auto_increment,
  `client_id` int(11) default NULL,
  `advisor_id` int(11) default NULL,
  `date` date default NULL,
  `survey_id` int(11) default NULL,
  `answer_list` varchar(255) default NULL,
  `totals` int(11) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSurveyItems`;
CREATE TABLE  `tblSurveyItems` (
  `id` int(11) NOT NULL auto_increment,
  `question` varchar(255) default NULL,
  `answer_txt` text,
  `answer_num` varchar(255) default NULL,
  `category` enum('E','S','M','I','G') default NULL,
  `creator_id` int(11) default NULL,
  `date` date default NULL,
  `status` enum('P','R','F','D') default 'P',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblSurveys`;
CREATE TABLE  `tblSurveys` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(64) default NULL,
  `description` varchar(255) default NULL,
  `question_list` varchar(255) default NULL,
  `creator_id` int(11) default NULL,
  `date` date default NULL,
  `status` enum('P','R','F','D') default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblTCredits`;
CREATE TABLE  `tblTCredits` (
  `id` int(11) NOT NULL auto_increment,
  `code` varchar(18) character set latin1 default NULL,
  `date` date NOT NULL default '0000-00-00',
  `branch_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `fund_id` int(11) NOT NULL,
  `amount` decimal(13,2) NOT NULL default '0.00',
  `principal` decimal(13,2) NOT NULL default '0.00',
  `fees` decimal(13,2) NOT NULL default '0.00',
  `insurances` decimal(13,2) NOT NULL default '0.00',
  `interest` decimal(13,2) NOT NULL default '0.00',
  `penalties` decimal(13,2) NOT NULL default '0.00',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `code` (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='ACCOUNTING';

DROP TABLE IF EXISTS `tblTDebits`;
CREATE TABLE  `tblTDebits` (
  `id` int(11) NOT NULL auto_increment,
  `code` varchar(32) NOT NULL,
  `date` date NOT NULL,
  `branch_id` int(11) NOT NULL,
  `program_id` int(11) NOT NULL,
  `amount` decimal(13,2) NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `code` (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblUserIOM`;
CREATE TABLE  `tblUserIOM` (
  `id` int(11) NOT NULL auto_increment,
  `type` enum('I','O') default NULL,
  `user_id` int(11) default NULL,
  `zone_id` tinyint(4) NOT NULL default '0',
  `memo` text,
  `creator_date` date default NULL,
  `creator_id` int(11) default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblUsers`;
CREATE TABLE  `tblUsers` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(16) default NULL,
  `password` varchar(35) default NULL,
  `force_next_pwd_date` date NOT NULL,
  `staff` enum('1','0') NOT NULL default '1',
  `creator_id` int(11) default NULL,
  `creator_date` date default NULL,
  `editor_id` int(11) default NULL,
  `editor_date` date default NULL,
  `access_code` int(11) default NULL,
  `super_id` int(11) default NULL,
  `code` varchar(13) default NULL,
  `first` varchar(16) default NULL,
  `middle` varchar(16) default NULL,
  `last` varchar(16) default NULL,
  `birthdate` date default NULL,
  `gender` enum('M','F') default NULL,
  `cstatus` enum('M','S','E','D','W') default NULL,
  `dependants` tinyint(2) default NULL,
  `email` varchar(64) default NULL,
  `zone_id` int(11) default NULL,
  `memo` text,
  `home` varchar(255) NOT NULL default '',
  `active` enum('1','0') NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `username` USING BTREE (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tblZones`;
CREATE TABLE  `tblZones` (
  `id` int(11) NOT NULL auto_increment,
  `parent_id` int(11) NOT NULL default '0',
  `zone` varchar(32) default NULL,
  `short_name` char(3) NOT NULL default '',
  `zA` varchar(32) default NULL,
  `zB` varchar(32) default NULL,
  `zC` varchar(32) default NULL,
  `memo` text,
  `program_id` int(11) NOT NULL default '0',
  `creator_id` int(11) NOT NULL default '0',
  `creator_date` date NOT NULL default '0000-00-00',
  `editor_id` int(11) NOT NULL default '0',
  `editor_date` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;