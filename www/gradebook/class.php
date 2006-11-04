<?php

int class_create(title, dept, course, section, term, year)
{
  if (!class_exists(dept, course, section, term, year))
    // Query string should contain properly formatted SQL
    query := 'INSERT title={title}, dept={dept}, course={course}, section={section}, term={term}, year={year}, categories={categories}, null INTO class'
    result := mysql(query)
    if (result != success)
      return failure
    return class_id
  else
    return failure
}
bool class_edit(class_id, title, dept, course, section, term, year)
{
  if (class_exists(class_id))
    // Query string should contain properly formatted SQL, will want to update     
    // only changed information
    query := 'UPDATE class SET title={title}, dept={dept}, course={course}, section={section}, term={term}, year={year}, categories={categories}, null WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_delete(class_id)
// We never want to delete things in most of these classes.
// If there were a large number of entries in the database it could wreak havoc to delete a class. So instead we will set a flag to hide a class. If a teacher really wants a class to disappear we might need a separate front end that is dedicated to hazardous operations
{
  if (class_exists(class_id))
    // Query string should contain properly formatted SQL
    query := 'UPDATE class SET hide=1 WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
array class_get(class_id)
{
  if (class_exists(class_id))
    // Query string should contain properly formatted SQL
    query := 'SELECT * FROM class WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return result
  else
    return failure
}
array class_get_all()
{
  // Query string should contain properly formatted SQL
  query := 'SELECT * FROM class'
  results := mysql(query)
  if (results != success)
    return failure
  else
    return results
}
bool class_exists(class_id)
{
  result := class_get(class_id)
  if (result != 0)
    return success
  else
    return failure
}

?>