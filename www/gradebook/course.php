<?php
bool course_create(course_id, dept_id, course_no, title)
{
  if (!course_exists(course_id))
    // Query string should contain properly formatted SQL
    query := 'INSERT course_id={course_id}, dept_id={dept_id}, course_no={course_no}, title={title} INTO course'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool course_edit(course_id, dept_id, course_no, title)
{
  if (course_exists(course_id))
    // Query string should contain properly formatted SQL, will want to update     
    // only changed information
    query := 'UPDATE course SET dept_id={dept_id}, course_no={course_no}, title={title} WHERE course_id={course_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure  
}
bool course_delete(course_id)
{
  if (course_exists(course_id))
    // Query string should contain properly formatted SQL
    query := 'DELETE FROM course WHERE course_id={course_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
array course_get(course_id)
{
  if (course_exists(course_id))
    // Query string should contain properly formatted SQL
    query := 'SELECT * FROM course WHERE course_id={course_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return result
  else
    return failure
}
array course_get_all()
{
  query := 'SELECT * FROM course'
  results := mysql(query)
  return results
}
bool course_exists(course_id)
{
  result := course_get(course_id)
  if (result != 0)
    return success
  else
    return failure
}
?>