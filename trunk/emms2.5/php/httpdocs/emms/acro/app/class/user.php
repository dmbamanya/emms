<?php
class USER extends WEBPAGE implements PERSONS, ROLES
{
  
  /* USER parameters */
  var $frostdata = array( 0 => 'username',
                          1 => 'creator_date',
                          2 => 'creator_date',
                          3 => 'active');
  var $userdata  = array( 'active'     => 1,
                          'creator_id' => 0);
  var $userfields = array();

  function USER($id = '')  //class constructor
  {
    if ($id)
    {
      $this->userdata                = current(SQL::select('tblUsers','*',sprintf('id=%s',$id)));
      $this->userdata['qjump']       = SQL::getAssoc('tblQjumps','qjump,qjump',sprintf('user_id=%s',$id));
      $this->userdata['roles']       = $this->getRoles($this->userdata['access_code']); //maskRoles(getRoles(),$user['access_code']);
      $this->userdata['verify']      = $this->userdata['password'];
      $this->userdata['oldpassword'] = $this->userdata['password'];
    } else {
      $this->userdata['creator_date'] = date('Y-m-d');
      $this->frostdata = array();
    } 
  }

  function getTemplateData($id)
  {
  $fields = sprintf("	
  				u.*,
				CONCAT(uc.first,' ',uc.last) AS creator, 
				DATE_FORMAT(u.creator_date,'%s') AS f_creator_date, 
				CONCAT(ue.first,' ',ue.last) AS editor, 
				DATE_FORMAT(u.editor_date,'%s') AS f_editor_date, 
				CONCAT(us.first,' ',us.last) AS super, 
				DATE_FORMAT(u.birthdate,'%s') AS f_birthdate, 
				z.zone", WEBPAGE::$gt['date_format_mysql'], WEBPAGE::$gt['date_format_mysql'], WEBPAGE::$gt['date_format_mysql']); 
  $tables = "tblUsers AS u, tblUsers AS us, tblUsers AS uc, tblUsers AS ue";
  $left   = "tblZones AS z";		   
  $on     = "u.zone_id = z.id";
  $param  = sprintf("u.id = '%s' AND u.super_id = us.id AND u.creator_id = uc.id AND u.editor_id = ue.id", $id);

  $data 				  = current(SQL::select_leftjoin($tables, $fields, $left, $on, $param));
    
  $data['super']          = WEBPAGE::hotlink('BS.SCR.viewUser','id',$data['super_id'],$data['super']);
  $data['name']           = sprintf("%s %s %s", $data['first'],$data['middle'],$data['last']);
  $data['status']         = WEBPAGE::$gt[sprintf('tblUsers.active.%s',$data['active'])];
  $data['zone']           = WEBPAGE::hotlink('BS.SCR.viewZone','id',$data['zone_id'],$data['zone']);
  $data['membership']     = self::getMembership($data['access_code']);
  $data['qjump']          = self::getQjumps($data['id']);

  $data['img_path']       = sprintf(WEBPAGE::_IMG_USER,$data['username']);
  
  if ((WEBPAGE::$conf['app'][auto_photo])&&(!(file_exists($data['img_path'])))) {
    $img_remote_path = sprintf("http://web.jce.do/consultas/FOTOS/%s/%s/%s/%s.jpg",substr($data['code'],0,3),substr($data['code'],4,2),substr($data['code'],6,2),$data['code']);
	
	if (!($fp_remote=@fopen($img_remote_path, "r"))) {
	  $data['img_path']	= './img/unknown.png';
	  } else {
      $data['img_path'] = $img_remote_path;
  	  }
/*
  		if ($fp_remote=fopen($img_remote_path, "r")) {
			$fp_local = fopen($img_path,"w");
			$image = fread($fp_remote,102400);
			fwrite($fp_local,$image,102400);
    		fclose($fp_remote);
    		fclose($fp_local);
			}
*/
    }

    $c=0;
	$data['buttondata'][$c][id]			="BS.SCR.addUser";
	$data['buttondata'][$c][href]		="index.php?scr_name=BS.SCR.addUser&id=".$id;
	$data['buttondata'][$c][alt]		= WEBPAGE::$gt['edit'];
	$data['buttondata'][$c][onClick]	="";
	$data['buttondata'][$c][ico]		="edit";

	if ($data['active']) {
	  $c++;
	  $data['buttondata'][$c][id]		="BS.SCR.suspendUser";
	  $data['buttondata'][$c][href]		="index.php?scr_name=BS.SCR.suspendUser&id=".$id;
	  $data['buttondata'][$c][alt]		= WEBPAGE::$gt['deactivate'];
	  $data['buttondata'][$c][onClick]	="";
	  $data['buttondata'][$c][ico]		="user_rem";
	  }

  return $data;
  }
  
  function roles($access_code = 255)  //implements interface ROLES
  {
  $roles = array();
  $opt = explode(",", WEBPAGE::$gt["tblUsers.access_code.optList"]);
  $nam = explode(",", WEBPAGE::$gt["tblUsers.access_code.optNames"]);
  $n = count($opt);
  for ($i=0; $i<$n; $i++) {
    $val = explode("|", $opt[$i]);
    if ($val[1] & intval($access_code)) {
      $roles[$i] = array('role'=>$val[0], 'label'=>$nam[$i], 'code'=>$val[1]);
      }
    }
  return $roles;
  }    

  function supervisors() //implements interface PERSONS
  {
  return SQL::getAssoc('tblUsers','id,username',sprintf("( access_code & %s ) != 0", self::SUPER));
  }

  function advisors($opt='') //implements interface PERSONS
  {
    if ($opt == '')
    {
      return SQL::getAssoc('tblUsers','id,username',sprintf("( access_code & %s ) != 0", self::ADVISOR));
    } else {
      return WEBPAGE::$dbh->getAssoc(sprintf("select max(u.id) id, concat(u.last,', ',u.first,' - ',u.username) username from tblUsers u, tblLoans l, view_loan_status vls where u.id = l.advisor_id and l.id = vls.loan_id and vls.status = '%s' group by u.username order by u.last",$opt));
    }

  }

  function getMyAdvisors($id = '') 
  {
  if (!($id)) { $id = $this->userdata['id']; }
  return SQL::getAssoc('tblUsers','id,username',sprintf("((super_id = '%s') OR (id = '%s')) AND ( access_code & %s ) != 0", $id, $id, self::ADVISOR));
  }

  function zones() //implements interface PERSONS
  {
  return SQL::getAssoc('tblZones','id,zone','parent_id = 0');
  }


  function getRoles($access_code) 
  {
  $roles = array();
  foreach (self::roles() as $key => $value) {
    if ($value['code'] & intval($access_code)) {
      $roles[$key] = $value['code'];
      }
    }
  return $roles;
  }

  function getMembership($access_code) 
  {
  $membership = array();
  $allroles = self::roles();
  foreach (self::getRoles($access_code) as $key => $value) {
    $membership[$key] = $allroles[$key]['label'];
    }
  return $membership;
  }
    
  function getName($userID) 
  {
  return current(SQL::getAssoc('tblUsers',"id, CONCAT(first, ' ', last)", sprintf("id = '%s'",$userID)));
  }

  function setQjumps($Qjumps)
  {
    foreach($Qjumps as $qjval)
    {
      $qjdata[] = sprintf('("%s","%s")',$this->userdata['id'],$qjval);
    }
    SQL::delete('tblQjumps','user_id = '.$this->userdata['id']);
    SQL::insert_mult('tblQjumps','user_id,qjump',implode(',',$qjdata));
  }

  function getQjumps($id)
  {
    return SQL::getAssoc('tblQjumps','qjump,qjump','user_id = '.$id);
  }

  function validatePassword($password)
  {
    $pwd = WEBPAGE::$dbh->getAssoc(sprintf("select id,password from tblUsers where id = '%s' and password = '%s'",$this->userdata['id'],crypt($password, md5($password))));
    if ($pwd[$this->userdata['id']]) return true;
    return false;
  }

  function validatePasswordUsageHistory($current_password,$new_password)
  {
    $pwd = WEBPAGE::$dbh->getAll(sprintf("select password pwd from tblPasswords where user_id = '%s' and password = '%s' and date < curdate() and date > date_sub(curdate(), INTERVAL %s DAY)",$this->userdata['id'],crypt($new_password, md5($new_password)),WEBPAGE::$conf['app']['pwd_diff_period']));
    if ( $pwd[0]['pwd'] ) { return false; }
    if (levenshtein($current_password,$new_password) < WEBPAGE::$conf['app']['pwd_min_diff']) return false;
    return true;
  }

  static function validatePasswordMetrics($password)
  {
  if (strlen($password) < WEBPAGE::$conf['app']['pwd_min_length']) { return false; }
  if (strlen($password) > WEBPAGE::$conf['app']['pwd_max_length']) { return false; }
  $pwd = str_split($password);
  $lowercase = 0;
  $uppercase = 0;
  $number    = 0;
  foreach($pwd as $char)
  {
    $lowercase = (strtoupper($char) != $char) ? $lowercase + 1 : $lowercase;
    $uppercase = (strtolower($char) != $char) ? $uppercase + 1 : $uppercase;
    $number    = (is_numeric($char))          ? $number + 1    : $number;
  }
  $special = count($pwd) - $lowercase - $uppercase - $number;
  if ($lowercase < WEBPAGE::$conf['app']['pwd_min_lowercase']) { return false; }
  if ($uppercase < WEBPAGE::$conf['app']['pwd_min_uppercase']) { return false; }
  if ($number    < WEBPAGE::$conf['app']['pwd_min_number'])    { return false; }
  if ($special   < WEBPAGE::$conf['app']['pwd_min_special'])   { return false; }
  return true;
  }
}
?>