import Image from "@theme/IdealImage";
import styles from "./styles.module.css";

export const GalleryGrid = ({ images }) => {
  return (
    <div className={styles.galleryGrid}>
      {images.map((ideal, i) => {
        const largest = ideal.src.images.reduce(
          (max, img) => (img.width > max.width ? img : max),
          ideal.src.images[0]
        );
        return (
          <a
            href={largest.path}
            target="_blank"
            rel="noopener noreferrer"
            key={i}
          >
            <Image img={ideal} />
          </a>
        );
      })}
    </div>
  );
};
