<?php
require_once 'class/roles.php';
class CLIENT extends WEBPAGE
{ 

  /* CLIENT parameters */
  var $frostdata = array();
  var $data  = array();

  function CLIENT($id = '')  //class constructor
  { 
  if ($id) {
  	/* No trabaja bien con clientes inactivos por lio con LEFT JOIN. Abajo esta resuelto.
    $tables = "tblClients AS c, tblUsers as uc, tblUsers as ue, tblUsers as ua, tblZones AS z, tblZones AS pz, tblPrograms AS p"; 
    $fields = sprintf("
				c.*, s.name AS society, s.category, 
				CONCAT(ua.first,' ',ua.last) AS advisor, 
				CONCAT(pz.zone,' : ',p.program) AS zone, 
				CONCAT(uc.first,' ',uc.last) AS creator, 
				DATE_FORMAT(c.creator_date,'%s') AS f_creator_date, 
				CONCAT(ue.first,' ',ue.last) AS editor, 
				DATE_FORMAT(c.editor_date,'%s') AS f_editor_date, 
				DATE_FORMAT(c.birthdate,'%s') AS f_birthdate",
				WEBPAGE::$gt['date_format_mysql'],WEBPAGE::$gt['date_format_mysql'],WEBPAGE::$gt['date_format_mysql']);
    $left   = "tblSocieties AS s";
    $on     = "c.society_id = s.id";
	$params = sprintf("c.id = '%s' AND c.creator_id = uc.id AND c.editor_id = ue.id AND c.advisor_id = ua.id AND z.id = c.zone_id AND pz.id = z.parent_id AND p.id = z.program_id", $id);
    $this->data 				= current(SQL::select_leftjoin($tables,$fields,$left,$on,$params));
	*/
		    
    $fields = sprintf("
				c.*, s.name AS society, s.category, 
				CONCAT(ua.first,' ',ua.last) AS advisor, 
				CONCAT(pz.zone,' : ',p.program) AS zone, 
				CONCAT(uc.first,' ',uc.last) AS creator, 
				DATE_FORMAT(c.creator_date,'%s') AS f_creator_date, 
				CONCAT(ue.first,' ',ue.last) AS editor, 
				DATE_FORMAT(c.editor_date,'%s') AS f_editor_date, 
				DATE_FORMAT(c.birthdate,'%s') AS f_birthdate",
				WEBPAGE::$gt['date_format_mysql'],WEBPAGE::$gt['date_format_mysql'],WEBPAGE::$gt['date_format_mysql']);
				
    $tables = "tblClients AS c, tblUsers as uc, tblUsers as ue";
         
    $this->data = current(WEBPAGE::$dbh->getAll(sprintf('
    			select %s from (%s)
    					left join tblSocieties 	as s 	on c.society_id 	= s.id
    					left join tblUsers 		as ua 	on c.advisor_id 	= ua.id
    					left join tblZones 		as z 	on z.id 			= c.zone_id
   						left join tblZones 		as pz 	on pz.id 			= z.parent_id
   						left join tblPrograms 	as p 	on z.program_id 	= p.id
     				where c.id = %s AND c.creator_id = uc.id AND c.editor_id = ue.id', $fields, $tables, $id)));
    

	$this->data['name']			= sprintf("%s, %s", $this->data['last'], $this->data['first']);
	$this->checkConfidentiality();
	$this->data['business']		= $this->getBusiness();

    if (!$this->data['category']) { $this->data['category'] = $this->data['advisor_id'] ? 'I' : ''; }

	switch ($this->data['category']) {
		CASE 'G':
			$this->data['borrower_id'] = $this->data['society_id'];
			break;
		DEFAULT:
			$this->data['borrower_id'] = $this->data['id'];
		}

    $this->setImgPath();

    $this->frostdata[] 			= 'society';
    $this->frostdata[] 			= 'society_id';
    $this->frostdata[] 			= 'advisor_id';
    $this->frostdata[] 			= 'zone_id';
    } else {

    $this->data['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getCreditHistory($id)
  {
    return WEBPAGE::$dbh->getAll(sprintf(' 
             select 
               l.id,
               l.loan_code,
               l.status,
               if((l.delivered_date != "0000-00-00"),l.delivered_date,"-") delivered_date,
               if(lsh.date,lsh.date,"-") cancel_date,
               l.kp,
               if(p.balance_kp,p.balance_kp,l.kp) balance_kp,
               if(p.delay,p.delay,0) delay,
               l.pmt,
               l.xp_num_pmt,
               lt.payment_frequency
             from 
               tblLoans l
             left join 
               tblLoanStatusHistory lsh on lsh.loan_id = l.id and lsh.status = "C"
             left join 
               view_loan_status_max_id lsmid on lsmid.loan_id = l.id
             left join
               tblLoanTypes lt on lt.id = l.loan_type_id
             left join
             (
               select max(id), sum(delay) delay, loan_id, balance_kp from tblPayments group by loan_id
             ) p on p.loan_id = l.id
             where
               l.client_id = %s
             order by
               l.id',$id));
  }

  function getTemplateData($id)
  {
  $client = new CLIENT($id);
  $data = $client->data;
  
  $data['society_code'] = sprintf("%s.%s.%s",
  							 str_pad($data['zone_id'],3,'0',STR_PAD_LEFT),
  							 $data['category'],
							 str_pad($data['society_id'],5,'0',STR_PAD_LEFT));
  $data['society_code']  	  = WEBPAGE::hotlink('BS.SCR.viewSociety','id',$data['society_id'],$data['society_code']);

  $zone = CLIENT::zones();
  $data['zone']           = $zone[$data['zone_id']];
  $cstatus = CLIENT::cstatus();
  $data['cstatus'] = $cstatus[$data['cstatus']];
  $education = CLIENT::education();
  $data['education'] = $education[$data['education']];

    $c=0;
	if ($data['advisor_id']) {
	  $data['buttondata'][$c]['id']		    = "BS.SCR.addClient";
	  $data['buttondata'][$c]['href']		  = "index.php?scr_name=BS.SCR.addClient&id=".$id;
	  $data['buttondata'][$c]['alt']		  = WEBPAGE::$gt['edit'];
	  $data['buttondata'][$c]['onClick']	= "";
	  $data['buttondata'][$c]['ico']		  = "edit";
	  $c++;
	  $data['buttondata'][$c]['id']		    = "LN.SCR.browseLoans";
	  $data['buttondata'][$c]['href']		  = "index.php?scr_name=LN.SCR.browseLoans&loan_code=.".str_pad($id,6,0,STR_PAD_LEFT);
	  $data['buttondata'][$c]['alt']		  = WEBPAGE::$gt['LN.SCR.browseLoans'];
	  $data['buttondata'][$c]['onClick']	= "";
	  $data['buttondata'][$c]['ico']		  = "money_history";
	  $c++;
	  $data['buttondata'][$c]['id']		    = "BS.SCR.moveClient";
	  $data['buttondata'][$c]['href']		  = "index.php?scr_name=BS.SCR.moveClient&ref=BS.SCR.editClient&id=".$id;
	  $data['buttondata'][$c]['alt']		  = WEBPAGE::$gt['moveClient'];
	  $data['buttondata'][$c]['onClick']	= "";
	  $data['buttondata'][$c]['ico']		  = "client_move";
	  $c++;
	  $data['buttondata'][$c]['id']		    = "BS.SCR.deactivateClient";
	  $data['buttondata'][$c]['href']		  = "index.php?scr_name=BS.SCR.deactivateClient&ref=BS.SCR.editClient&id=".$id;
	  $data['buttondata'][$c]['alt']		  = WEBPAGE::$gt['deactivate'];
	  $data['buttondata'][$c]['onClick']	= "";
	  $data['buttondata'][$c]['ico']		  = "client_rem";
	  $c++;
      $data['buttondata'][$c]['id']	     = "BS.SCR.addBusiness";
      $data['buttondata'][$c]['href']		 = "index.php?scr_name=BS.SCR.addBusiness&owner_id=".$id;
      $data['buttondata'][$c]['alt']		 = WEBPAGE::$gt['addBusiness'];
      $data['buttondata'][$c]['onClick'] = "";
      $data['buttondata'][$c]['ico']		 = "business_add";
      $c++;
      $data['buttondata'][$c]['id']	     = "SV.SCR.applySurveyStepB";
      $data['buttondata'][$c]['href']		 = "index.php?scr_name=SV.SCR.browseSurveys&ref=SV.SCR.applySurveyStepA&client_id=".$id;
      $data['buttondata'][$c]['alt']		 = WEBPAGE::$gt['applySurvey'];
      $data['buttondata'][$c]['onClick'] = "";
      $data['buttondata'][$c]['ico']		 = "survey_apply";
      $c++;
      $data['buttondata'][$c]['id']	     = "SV.SCR.viewGraph";
      $data['buttondata'][$c]['href']		 = "index.php?scr_name=SV.SCR.browseSurveys&stats_client_id=".$id;
      $data['buttondata'][$c]['alt']		 = WEBPAGE::$gt['SV.SCR.viewGraph'];
      $data['buttondata'][$c]['onClick'] = "";
      $data['buttondata'][$c]['ico']		 = "survey_results";
      $c++;
	  } else {
	  $data['buttondata'][$c]['id']		     = "BS.SCR.addClient";
	  $data['buttondata'][$c]['href']		   = "index.php?scr_name=BS.SCR.addClient&ref=BS.SCR.editClient&activate=1&id=".$id;
	  $data['buttondata'][$c]['alt']		   = WEBPAGE::$gt['activate'];
	  $data['buttondata'][$c]['onClick']	 = "";
	  $data['buttondata'][$c]['ico']		   = "client_add";
	  $c++;	  
	  }

	  $data['buttondata'][$c]['id']		     = "BS.SCR.viewClientIOM";
	  $data['buttondata'][$c]['href']		   = 'javascript:openWin("index.popup.php?scr_name=BS.SCR.viewClientIOM&ref=BS.SCR.viewClient&id='.$id.'","ClientIOM","menubar=no,scrollbars=no,resizable=no,width=700,height=320")';
	  $data['buttondata'][$c]['alt']		   = WEBPAGE::$gt['movements'];
	  $data['buttondata'][$c]['onClick']	 = "";
	  $data['buttondata'][$c]['ico']		   = "client_history";
	  $c++;	  

  return $data;
  }
  
  function advisors() 
  {
  return SQL::getAssoc_order('tblUsers','id,CONCAT(last,", ",first, " ",middle)',sprintf("( access_code & %s ) != 0", ROLES::ADVISOR),'last');
  }

  function zones() 
  {
  return SQL::getAssoc_order('tblZones as z, tblPrograms as p, tblZones as pz','z.id,CONCAT(pz.zone," : ",p.program)','z.parent_id !=0 AND p.id = z.program_id AND pz.id = z.parent_id','z.parent_id');
  }

  function education()
  {
  $opt = explode(",", WEBPAGE::$gt['tblClients.education.optList']);
  $nam = explode(",", WEBPAGE::$gt['tblClients.education.optNames']);
  foreach ($opt as $key => $val) {
    $edu[$val] = $nam[$key];
    }
  return $edu;
  }

  function cstatus()
  {
  $opt = explode(",", WEBPAGE::$gt['tblClients.cstatus.optList']);
  $nam = explode(",", WEBPAGE::$gt['tblClients.cstatus.optNames']);
  foreach ($opt as $key => $val) {
    $cs[$val] = $nam[$key];
    }
  return $cs;
  }
  
  function iomcauses()
  {
  $opt = explode(",", WEBPAGE::$gt['tblClientIOM.cause.optList']);
  $nam = explode(",", WEBPAGE::$gt['tblClientIOM.cause.optNames']);
  foreach ($opt as $key => $val) {
    $cause[$val] = $nam[$key];
    }
  return $cause;
  }

  function checkConfidentiality()
  {
  return true;
  $tables = 'tblClientIOM as iom, tblZones as z, tblPrograms as p';
  $fields = 'count(iom.id) as confidential';
  $param = sprintf("iom.client_id = '%s' 
                and iom.zone_id = z.id
                and z.program_id = p.id
                and p.confidential = '1'",$this->data['id']);
/*
  $tables = "tblConfidentials AS cn, tblClientIOM AS iom, tblClients AS c";
  $tables = "tblProgramas AS cn, tblClientIOM AS iom, tblClients AS c, tblZones AS z";
  $fields = "cn.confidential_id AS confidential, c.advisor_id AS advisor_id";
  $fields = "cn.id AS confidential, c.advisor_id AS advisor_id";
  $param  = sprintf("cn.type = 'Z'
					AND iom.client_id = '%s'
					AND iom.zone_id = cn.confidential_id
					AND c.id = iom.client_id", $this->data['id']);
  $param  = sprintf("   iom.client_id = '%s'
					AND iom.zone_id = z.id
					AND cn.id = z.program_id
					AND c.id = iom.client_id
				    AND cn.confidential = '1'", $this->data['id']);
*/
  $crow = current(SQL::select($tables,$fields,$param));
  if ( ( $crow['confidential'] )&&( $this->data['advisor_id'] != self::$userID )&&(!(self::$userAccessCode & ROLES::ADMIN))  ) {
	$this->data['confidential'] 	= true;
	$this->data['code'] 			= WEBPAGE::$gt['confidential'];
	$this->data['name'] 			= WEBPAGE::$gt['confidential'];
	$this->data['first']			= WEBPAGE::$gt['confidential'];
	$this->data['middle'] 			= WEBPAGE::$gt['confidential'];
	$this->data['last'] 			= WEBPAGE::$gt['confidential'];
	$this->data['nick'] 			= WEBPAGE::$gt['confidential'];
	$this->data['spouse'] 			= WEBPAGE::$gt['confidential'];
   	$this->data['birthdate'] 		= WEBPAGE::$gt['confidential'];
   	$this->data['birthdate_f2'] 	= WEBPAGE::$gt['confidential'];
	$this->data['memo'] 			= WEBPAGE::$gt['confidential'];
	}  
  } 
  
  function setImgPath()
  {
  if (file_exists(sprintf("./img/clients/%s.jpg",$this->data['code']))) {
    $this->data['img_path']		  = sprintf(WEBPAGE::_IMG_CLIENT,$this->data['code']);
    } else {
    $this->data['img_path']	= './img/unknown.png';	
    }

//  if ((WEBPAGE::$conf['app'][auto_photo])&&(!(file_exists($this->data['img_path'])))) {
//    $img_remote_path = sprintf("http://web.jce.do/consultas/FOTOS/%s/%s/%s/%s.jpg",substr($this->data['code'],0,3),substr($this->data['code'],4,2),substr($this->data['code'],6,2),$this->data['code']);

//	if (!($fp_remote=@fopen($img_remote_path, "r"))) {
//	  $this->data['img_path']	= './img/unknown.png';
//	  } else {
//     $this->data['img_path'] = $img_remote_path;
//  	  }
//    }
  }

  function getBusiness()
  {
  return SQL::getAssoc("tblBusiness AS b, tblClients as c", "b.id, b.name", sprintf("c.id = '%s' AND FIND_IN_SET(c.id,b.client_list)",$this->data['id']));
  }
      
  function loans($ClientID,$LoanStatus = '%%',$BorrowerType='B') 
  {	
  if ($LoanStatus == '%%') {$operator = 'LIKE';} else {$operator = '=';}
  
  $tables = 'tblLoans as l, tblLoanTypes AS lt';
  $fields = 'l.id, l.status';
  $params = sprintf("l.client_id = %s 
							AND l.status %s '%s'
							AND l.loan_type_id = lt.id
							AND lt.borrower_type = '%s'",
							$ClientID,$operator,$LoanStatus,$BorrowerType);
  $mrow = SQL::select($tables,$fields,$params);
  $num = count($mrow);
  for ($i=0; $i<$num; $i++) {
    $data[$mrow[$i]['status']][] = $mrow[$i]['id'];
    }		  	
  return $data;
  }

}
?>