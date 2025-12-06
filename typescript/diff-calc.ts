// A little calculator for automatic yearly incrementing my years of experience from my personal
// homepage.

function getYearsOfExperience(startYear: number): number {
  return new Date().getFullYear() - startYear;
}

const yearsOfExperience = getYearsOfExperience(2018);

// Use it like:
//  <p>
//    ...than {yearsOfExperience} years ...
//  </p>
