<?php

require("../../choosy/xml_parser.php");

class optionlist {
    function optionlist($xmlfile) {
        $data = file_get_contents($xmlfile);
        $x =& new xml2Array();
        $this->dom = $x->parse($data);
        unset($data);
    }
    function &get_list() {
        $r = array(); //return array, since php doesn't support `yield`
        $root =& $this->dom['OPTIONS'][0];
        foreach($root['SECTION'] as $section) {
            foreach($section['OPTION'] as $opt) {
                if(isset($opt['attributes']['TYPE']) && $opt['attributes']['TYPE']=="single_exclusion") {
                    //yay, we get to handle a special one
                    $r[] =& new radiooption($opt);
                } else {
                    $r[] =& new regularoption($opt);
                }
            }
        }
        return $r;
    }
    function &get_section_list() {
        $r = array();
        $root =& $this->dom['OPTIONS'][0];
        foreach($root['SECTION'] as $section) {
            $r[] = new optionsection($section);
        }
        return $r;
    }
    
    function &get_model_list() {
        $r = array(); //return array, since php doesn't support `yield`
        $root =& $this->dom['OPTIONS'][0]['MODELS'][0];
        foreach($root['OPTION'] as $opt) {
            $r[] =& new modeloption($opt);
        }
        if(!count($r)) return false;
        return $r;
        
    }
    function &get_model($name) {
        $models =& $this->get_model_list();
        foreach($models as $m) {
            if ($m->id==$name)
                return $m;
        }
        return false;
    }
    function get_characteristics_list() {
        $r = array(); //return array, since php doesn't support `yield`
        $root =& $this->dom['OPTIONS'][0]['MODELS'][0]['CHARACTERISTICS'][0];
        foreach($root['CHARACTERISTIC'] as $characteristic) {
            $r[] =& new charoption($characteristic);
        }
        if(!count($r)) return false;
        return $r;
        
    }
    function get_characteristic($name) {
        $characteristics =& $this->get_characteristics_list();
        foreach($characteristics as $c) {
            if ($c->id==$name)
                return $c;
        }
        return false;
    }
}

class baseoption {
    //pass
    function _guess_imagename($id=false) {
        if($id==false) $id=$this->name;
        $img = $id;
        if(isset($this->altimage)) $img = $this->altimage;
        //echo "Picture: i/aliner/aliner_product_photos/{$img}.jpg\n\n";
        return "i/aliner/aliner_product_photos/{$img}.jpg";
    }
}

class regularoption extends baseoption {
    function regularoption($domnode) {
        //$this->data =& $domnode;
        $this->name = $domnode['attributes']['ID'];
        $this->title = $domnode['NAME'][0]['DATA'];
        $this->id = $domnode['attributes']['ID'];
        $this->cost = $domnode['COST'][0]['DATA'];
        if(isset($domnode['ALTIMAGE'])) $this->altimg = $domnode['ALTIMAGE'][0]['DATA'];
        if(isset($domnode['BARKER'])) $this->barker = $domnode['BARKER'][0]['DATA'];
        if(isset($domnode['REQUIRES'])) {
            $this->requires = array();
            foreach($domnode['REQUIRES'][0]['REQUIRE'] as $x) {
                $this->requires[] = $x['DATA'];
            }
        }
        if(isset($domnode['INCLUDES'])) {
            $this->includes = array();
            foreach($domnode['INCLUDES'][0]['INCLUDE'] as $x) {
                $this->includes[] = $x['DATA'];
            }
        }
        //requires, etc... maybe needs a factory, ugh
    }
    function get_image($id=false) {
        global $root_dir;
        $path = $this->_guess_imagename();
            if(file_exists($root_dir."/".$path)) return $path;
        return false;
    }
    function has_image() {
        return ($this->get_image() !== false);
    }
    function is_selected() {
        return (isset($_REQUEST[$this->id]));
    }
    function add_cost(&$total) {
        if($this->is_selected()) $total += $this->cost;
    }
    function get_cost() {
        return $this->cost;
    }
    function to_str() {
        return $this->title;
    }
    function get_title() {
        return $this->title;
    }
    function get_title_tag() {
        $pieces = array();
        
        // Does it have barker text?
        if(isset($this->barker)) {
            $pieces[] = sanitize_attribute($this->barker);
        }
        
        //echo "What is my name again? - ".$this->get_image()."\n\n";
        //var_dump($this->get_image());
        if(($imgpath=$this->get_image()) !== false) {
            global $ptr, $root_dir;
            list($width, $height) = getimagesize($root_dir."/".$imgpath);
            //echo "Getting title {$ptr}{$imgpath}\n\n";
            $pieces[] = "[img src='{$ptr}{$imgpath}' width='{$width}' height='{$height}' /]";
        }
        
        if(!count($pieces)) return "";
        return "title=\"".join("[br /][br /]", $pieces)."\"";
    }
    function is_applicable(&$model) {
        return in_array($this->name, $model->allows, true);
    }
    function to_htmlform() {
        //this outputs the input elements, selected as appropriate, etc
        $hasimg = "";
        if($this->has_image()) $hasimg="&#8251;";
        $title = $this->get_title_tag();
        $price = $this->cost;
        if(is_numeric($price)) $price = sprintf('$%0.02f', $price);
        return "<div class=\"splitty\"><label class=\"semi roundy\" for=\"{$this->id}\" {$title}>
    <span class=\"price\">{$price}</span>
    <input type=\"checkbox\" name=\"{$this->id}\" id=\"{$this->id}\" style=\"vertical-align: middle\" />
    {$this->title}{$hasimg}
    <b class=\"c tl\">&nbsp;</b><b class=\"c tr\">&nbsp;</b><b class=\"c br\">&nbsp;</b><b class=\"c bl\">&nbsp;</b>
</label></div>
";
    }
    function to_js() {
        //this outputs the bag[] = {} stuff
        if(!isset($this->cost) || ($this->cost==0)) return "";
        $els = array();
        $els[] = "\"price\": {$this->cost}";
        if(isset($this->requires)) {
            $stuff = array();
            foreach($this->requires as $r) {
                $stuff[] = "\"{$r}\"";
            }
            $els[] = "\"requires\": [".join(",", $stuff)."]";
        }
        if(isset($this->includes)) {
            $stuff = array();
            foreach($this->includes as $r) {
                $stuff[] = "\"{$r}\"";
            }
            $els[] = "\"includes\": [".join(",", $stuff)."]";
        }
        return "bag.{$this->id} = \{ ".join(", ", $els)." };\n";
    }
}

class radiooption extends baseoption {
    function radiooption($domnode) {
        //$this->data =& $domnode;
        $this->name = $domnode['attributes']['ID'];
        $this->title = $domnode['NAME'][0]['DATA'];
        $this->ids = array();
        $this->names = array();
        $this->costs = array();
        foreach($domnode['SUBOPTION'] as $i=>$so) {
            $this->ids[$i] = $so['attributes']['ID'];
            $this->names[$i] = $so['NAME'][0]['DATA'];
            $this->costs[$i] = $so['COST'][0]['DATA'];
            if(isset($so['BARKER'])) $this->barker[$i] = $so['BARKER'][0]['DATA'];
            if(isset($so['attributes']['DEFAULT']) && $so['attributes']['DEFAULT']=="true") {
                $this->def = $i;
            }
        }
        //requires, etc... maybe needs a factory, ugh
    }
    
    function get_image($id) {
        $path = $this->_guess_imagename($id);
        if(file_exists($path)) return $path;
        return false;
    }
    function has_image($index) {
        return ($this->get_image($this->ids[$index]) !== false);
    }
    
    //beware numeric zero vs false, see the === operator
    function is_selected() {
        if(isset($_REQUEST[$this->name])) {
            $i = array_search($_REQUEST[$this->name], $this->ids);
            return $i;
        }
        return false;
    }
    
    //note this behaves oddly, if none are selected, it returns 0
    function get_cost() {
        if(($v=$this->is_selected()) !== false) {
            return $this->costs[$v];
        }
        return 0;
    }
    
    function add_cost(&$total) {
        if(($v=$this->is_selected()) !== false) {
            $total += $this->costs[$v];
        }
    }
    
    function to_str() {
        return $this->name;
    }
    
    function get_title() {
        if(($v=$this->is_selected()) !== false) {
            return sprintf("%s: %s", $this->title, $this->names[$v]);
        }
    }

    function get_title_tag($i) {
        $pieces = array();
        
        // Does it have barker text?
        if(isset($this->barker[$i])) {
            $pieces[] = sanitize_attribute($this->barker[$i]);
        }
        
        if(($imgpath=$this->get_image($i)) !== false) {
            list($width, $height) = getimagesize($imgpath);
            $pieces[] = "[img src='{$imgpath}' width='{$width}' height='{$height}' /]";
        }
        
        if(!count($pieces)) return "";
        return "title=\"".join("[br /][br /]", $pieces)."\"";
    }

    function is_applicable(&$model) {
        return in_array($this->name, $model->allows, true);
    }
    
    function to_htmlform() {
        //this outputs the input elements, selected as appropriate, etc
        $r = "";
        $r.="<fieldset>\n";
        $r.="<legend>{$this->title}</legend>\n";
        foreach(array_keys($this->ids) as $i) {
            $hasimg = "";
            $id = $this->ids[$i];
            if($this->has_image($i)) $hasimg="&#8251;";
            $title = $this->get_title_tag($i);
            $price = $this->costs[$i];
            if(is_numeric($price)) $price = sprintf('$%0.02f', $price);
            $sel = ($this->def == $i ? " checked=\"checked\"" : "");
    
            $r.="<div class=\"splitty\"><label class=\"semi roundy\" for=\"{$id}\" {$title}>
    <span class=\"price\">{$price}</span>
    <input type=\"radio\" name=\"{$this->name}\" id=\"{$id}\" value=\"{$id}\" style=\"vertical-align: middle\"{$sel} />
    {$this->names[$i]}{$hasimg}
    <b class=\"c tl\">&nbsp;</b><b class=\"c tr\">&nbsp;</b><b class=\"c br\">&nbsp;</b><b class=\"c bl\">&nbsp;</b>
</label></div>
";
        }
        
        $r.="</fieldset>\n";
        return $r;
    }
    function to_js() {
        //this outputs the bag[] = {} stuff
        $r = array();
        foreach(array_keys($this->ids) as $i) {
            if(!isset($this->costs[$i]) || ($this->costs[$i]==0)) continue;
            $r[] = "bag.{$this->ids[$i]} = \{ \"price\": {$this->costs[$i]} };\n";
        }
        return join("", $r);
    }
}

class charoption {
    function charoption($domnode) {
        $this->id = $domnode['attributes']['ID'];
        $this->name = $domnode['DATA'];
    }
    function had_by_ashtml(&$model) {
        if(isset($model->char_string[$this->id])) {
            if($model->char_string[$this->id] == "yes please") {
                $x = "&bull;";
            }
            else $x=$model->char_string[$this->id];
        }
        else $x="&nbsp;";
        return "<td>{$x}</td>";
    }
}

class modeloption {
    function modeloption($domnode) {
        $this->id = $domnode['attributes']['ID'];
        $this->name = $domnode['NAME'][0]['DATA'];
        $this->cost = $domnode['COST'][0]['DATA'];
        $this->allows = array();
        $this->includes = array();
        $this->char_string = array();
        foreach($domnode['INCLUDES'][0]['INCLUDE'] as $a) {
            $this->includes[] = $a['DATA'];
        }
        foreach($domnode['ALLOWS'][0]['ALLOW'] as $a) {
            $this->allows[] = $a['DATA'];
        }
        foreach($domnode['CHARACTERISTICS'][0]['CHARACTERISTIC'] as $a) {
            $this->char_string[$a['attributes']['ID']] = $a['DATA'];
        }
    }
}

class optionsection {
    function optionsection($stuff) {
        $this->options = array();
        $this->name = $stuff['attributes']['NAME'];
        foreach($stuff['OPTION'] as $opt) {
            if(isset($opt['attributes']['TYPE']) && $opt['attributes']['TYPE']=="single_exclusion") {
                //yay, we get to handle a special one
                $this->options[] =& new radiooption($opt);
            } else {
                $this->options[] =& new regularoption($opt);
            }
        }
    }
    function &get_list() {
        return $this->options;
    }
    function to_str() {
        return $this->name;
    }
}

?>